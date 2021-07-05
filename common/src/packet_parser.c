#include "stdint.h"
#include "common/packet_parser.h"
#include "string.h"
#include "tx_api.h"

#define LOG_MODULE "packet_parser"
#include "log.h"

static bool_t _packet_parser_schema_compare(PacketSchema *a, PacketSchema *b);

static void _packet_parser_frame_pack(PacketParser *parser, PacketFrame *parsedFrame);

static uint32_t Command_CalculateOverHeadLength(PacketSchema schema);

static int8_t _packet_parser_block_clear(PacketParser *parser);

static int8_t _packet_parser_schema_check(PacketSchema *schema);

static bool_t _packet_parser_chars_seek(PacketParser *parser, char *pattern, int8_t *lps, uint8_t patternSize);

static int8_t _packet_parser_int_scan(PacketParser *parser, uint8_t size, uint32_t *value);

static int8_t _packet_parser_content_scan(PacketParser *parser, uint32_t length, uint32_t *scanedLength);

static void _packet_parser_context_init(PacketParser *parser, PacketSchema *schema);

static void _packet_parser_context_preparing(PacketParser *parser);

/**
 * @arg
 * @arg 
 * @return 0=success, -1=dismatch, 1=not enough chars
 * */
static int8_t _packet_parser_chars_scan(PacketParser *parser, char *pattern, uint8_t size);

static void _packet_parser_pattern_nexts_generate(char *p, uint8_t M, int8_t *next);

int8_t packet_parser_create(PacketParser *parser, char *name,
                            PacketSchema *schema,
                            RingBuffer *buffer)
{

    uint8_t checkResult = _packet_parser_schema_check(schema);
    if (checkResult != 0)
    {
        return checkResult;
    }

    parser->schema = schema;

    _packet_parser_pattern_nexts_generate(schema->prefix, schema->prefixSize, parser->_prefixPatternNexts);

    if (schema->suffixSize != 0)
    {
        _packet_parser_pattern_nexts_generate(schema->suffix, schema->suffixSize, parser->_suffixPatternNexts);
    }

    parser->buffer = buffer;
    parser->_stage = PACKET_PARSE_STAGE_INIT;
    return 0;
};

OP_RESULT packet_parser_get(PacketParser *parser, PacketSchema *customSchema, PacketFrame *parsedFrame)
{
    uint8_t stage = parser->_stage;
    uint8_t frameCount = 0;
    PacketSchema *schema = parser->_curSchema;

    if (customSchema != NULL)
    {
        // 提供了自定义schema. 如果提供的和当前的一致, 则沿用; 否则初始化.
        if (_packet_parser_schema_compare(customSchema, schema))
        {
        }
        else
        {
            schema = customSchema;
            stage = PACKET_PARSE_STAGE_INIT;
        }
    }
    else
    {
        if (parser->schema == schema)
        {
        }
        else
        {
            // 没提供自定义schema. 使用默认schema, 如果当前schema就是默认schema, 则沿用; 否则初始化.
            schema = parser->schema;
            stage = PACKET_PARSE_STAGE_INIT;
        }
    }
    PACKET_SCHEMA_MODE mode = schema->mode;
    bool_t result = false;
    switch (stage)
    {
    case PACKET_PARSE_STAGE_INIT:
        _packet_parser_context_init(parser, schema);
        stage = PACKET_PARSE_STAGE_SEEKING_PREFIX;
        // break; // Go to next stage directly.
    case PACKET_PARSE_STAGE_PREPARING:
        _packet_parser_context_preparing(parser);
        stage = PACKET_PARSE_STAGE_SEEKING_PREFIX;
        // break; // Go to next stage directly.
    case PACKET_PARSE_STAGE_SEEKING_PREFIX:
        result = _packet_parser_chars_seek(parser, schema->prefix, parser->_prefixPatternNexts, schema->prefixSize);
        if (result)
        {
            ringbuffer_read_offset_sync(parser->buffer, parser->_seekOffset - schema->prefixSize);
            parser->_packetStartOffset = 0;
            parser->_seekOffset = schema->prefixSize;
            stage = PACKET_PARSE_STAGE_PARSING_LENGTH;
        }
        else
        {
            stage = PACKET_PARSE_STAGE_SEEKING_PREFIX;
            break;
        }
    case PACKET_PARSE_STAGE_PARSING_LENGTH:
        if (mode == PACKET_SCHEMA_MODE_FIXED_LENGTH)
        {
            parser->_frameExpectContentLength = schema->fixed.fixedLength;
            parser->_frameActualContentLength = 0;
            stage = PACKET_PARSE_STAGE_SEEKING_CONTENT;
        }
        else if (mode = PACKET_SCHEMA_MODE_DYNAMIC_LENGTH)
        {
            result = _packet_parser_int_scan(parser, schema->dynamic.lengthSize, &parser->_frameExpectContentLength);
            if (result)
            {
                parser->_frameActualContentLength = 0;
                stage = PACKET_PARSE_STAGE_SEEKING_CONTENT;
            }
            else
            {
                stage = PACKET_SCHEMA_MODE_DYNAMIC_LENGTH;
                break;
            }
        }
        else
        {
            parser->_frameExpectContentLength = -1;
            stage = PACKET_PARSE_STAGE_SEEKING_SUFFIX;
        }
    case PACKET_PARSE_STAGE_SEEKING_CONTENT:
        if ((mode != PACKET_SCHEMA_MODE_FREE_LENGTH) && (parser->_frameExpectContentLength > 0))
        {
            uint32_t parsedLength = 0;
            result = _packet_parser_content_scan(parser, parser->_frameExpectContentLength - parser->_frameActualContentLength, &parsedLength);
            parser->_frameActualContentLength += parsedLength;
            if (result)
            {
                stage = PACKET_PARSE_STAGE_MATCHING_SUFFIX;
            }
            else
            {
                stage = PACKET_PARSE_STAGE_SEEKING_CONTENT;
                break;
            }
        }
    case PACKET_PARSE_STAGE_MATCHING_SUFFIX:
        if (mode != PACKET_SCHEMA_MODE_FREE_LENGTH)
        {
            if (schema->suffixSize == 0)
            {
                stage = PACKET_PARSE_STAGE_DONE;
            }
            else
            {
                result = _packet_parser_chars_scan(parser, schema->suffix, schema->suffixSize);
                if (result == 1)
                {
                    // success
                    stage = PACKET_PARSE_STAGE_DONE;
                }
                else if (result == 0) // not enough buf
                {
                    break;
                }
                else // mismatch
                {
                    ringbuffer_read_offset_sync(parser->buffer, parser->_seekOffset);
                    parser->_packetStartOffset = 0;
                    parser->_seekOffset = 0;
                    stage = PACKET_PARSE_STAGE_PREPARING;
                    result = 0;
                    break;
                }
            }
        }

    case PACKET_PARSE_STAGE_SEEKING_SUFFIX:
        if (schema->mode == PACKET_SCHEMA_MODE_FREE_LENGTH)
        {
            result = _packet_parser_chars_seek(parser, schema->suffix, parser->_suffixPatternNexts, schema->suffixSize);
            if (result)
            {
                parser->_frameActualContentLength = parser->_seekOffset - parser->_packetStartOffset - schema->suffixSize - schema->prefixSize;
                stage = PACKET_PARSE_STAGE_DONE;
            }
            else
            {
                break;
            }
        }
    case PACKET_PARSE_STAGE_DONE:
        _packet_parser_frame_pack(parser, parsedFrame);
        stage = PACKET_PARSE_STAGE_PREPARING;
        result = true;
        break;
    default:
        break;
    }

    parser->_stage = stage;
    if (result)
    {
        return OP_RESULT_OK;
    }
    else
    {
        return OP_RESULT_GENERAL_ERROR;
    }
};

static int8_t _packet_parser_schema_check(PacketSchema *schema)
{
    if (schema->prefixSize == 0)
    {
        LOG_E("prefix size=0");
        return -1;
    }
    switch (schema->mode)
    {
    case PACKET_SCHEMA_MODE_FIXED_LENGTH:

        if (schema->fixed.fixedLength == 0)
        {
            LOG_E("fixed mode: fixed length=0");
            return -1;
        }
        break;
    case PACKET_SCHEMA_MODE_DYNAMIC_LENGTH:

        break;
    case PACKET_SCHEMA_MODE_FREE_LENGTH:

        if (schema->suffixSize == 0)
        {
            LOG_E("free mode: suffix size=0");
            return -1;
        }
        break;
    default:
        break;
    }

    return 0;
};

static void _packet_parser_pattern_nexts_generate(char *p, uint8_t M, int8_t *next)
{
    next[0] = 0;
    int k = 0;
    for (int q = 1; q < M; q++)
    {
        while (k > 0 && p[k] != p[q])
        {
            k = next[k];
        }
        if (p[k] == p[q])
        {
            k++;
        }
        next[q] = k;
    }

    // while (i < M)
    // {
    //     if (j == -1 || p[i] == p[j])
    //     {
    //         ++i;
    //         ++j;
    //         next[i] = j;
    //     }
    //     else
    //     {
    //         j = next[j];
    //     }
    // }
};

static void _packet_parser_context_init(PacketParser *parser, PacketSchema *schema)
{
    parser->_seekOffset = 0;
    parser->_curSchema = schema;
};

static void _packet_parser_context_preparing(PacketParser *parser)
{
    parser->_frameActualContentLength = 0;
    parser->_frameExpectContentLength = 0;
    parser->_patternMatchedCount = 0;
};

static bool_t _packet_parser_schema_compare(PacketSchema *a, PacketSchema *b)
{
    bool_t rst;
    rst = a == b;
    if (rst)
    {
        return true;
    }
    if (a == NULL || b == NULL)
    {
        return false;
    }
    rst = a->mode == b->mode &&
          a->prefixSize == b->prefixSize &&
          a->suffixSize == b->suffixSize &&
          a->lengthIncludeCrc == b->lengthIncludeCrc &&
          a->crc == b->crc;
    if (!rst)
    {
        return false;
    }

    switch (a->mode)
    {
    case PACKET_SCHEMA_MODE_FIXED_LENGTH:

        rst = a->fixed.fixedLength == b->fixed.fixedLength;
        break;
    case PACKET_SCHEMA_MODE_DYNAMIC_LENGTH:

        break;
    case PACKET_SCHEMA_MODE_FREE_LENGTH:

        rst = a->dynamic.lengthSize == b->dynamic.lengthSize;
        break;
    default:
        break;
    }
    if (!rst)
    {
        return false;
    }
    for (uint8_t i = 0; i < a->prefixSize; i++)
    {
        if (a->prefix[i] != b->prefix[i])
        {
            return false;
        }
    }
    for (uint8_t i = 0; i < a->suffix; i++)
    {
        if (a->suffix[i] != b->suffix[i])
        {
            return false;
        }
    }
    return true;
};

static void _packet_parser_frame_pack(PacketParser *parser, PacketFrame *parsedFrame)
{
    PacketSchema *schema = parser->_curSchema;
    parsedFrame->length = parser->_seekOffset;
    if (schema->mode == PACKET_SCHEMA_MODE_DYNAMIC_LENGTH)
    {
        parsedFrame->contentStartOffset = schema->prefixSize + schema->dynamic.lengthSize;
    }
    else
    {
        parsedFrame->contentStartOffset = schema->prefixSize;
    }
    parsedFrame->contentLength = parser->_frameActualContentLength;
};

static inline int8_t _packet_parser_content_scan(PacketParser *parser, uint32_t expectLength, uint32_t *scanedLength)
{
    RingBuffer *buffer = parser->buffer;
    uint32_t totalLength = ringbuffer_count_get(buffer);
    int32_t seekOffset = parser->_seekOffset;

    if ((totalLength - seekOffset) >= expectLength)
    {
        parser->_seekOffset = seekOffset + expectLength;
        *scanedLength = expectLength;
        return true;
    }
    else
    {
        parser->_seekOffset = totalLength;
        *scanedLength = totalLength - seekOffset;
        return false;
    }
};

static int8_t _packet_parser_int_scan(PacketParser *parser, uint8_t size, uint32_t *value)
{
    RingBuffer *buffer = parser->buffer;
    uint32_t totalLength = ringbuffer_count_get(buffer);
    int32_t seekOffset = parser->_seekOffset;
    if ((totalLength - seekOffset) < size)
    {
        return false;
    }

    int32_t tmpValue = 0;
    while (size-- > 0)
    {
        uint32_t seekIndex = ringbuffer_offset_to_index_convert(buffer, seekOffset);
        tmpValue = (tmpValue << 8) + *(uint8_t *)ringbuffer_index_peek_directly(buffer, seekIndex);
        seekOffset++;
    }
    *value = tmpValue;

    parser->_seekOffset = seekOffset;
    return true;
};

static bool_t _packet_parser_chars_seek(PacketParser *parser, char *pattern, int8_t *next, uint8_t patternSize)
{
    RingBuffer *buffer = parser->buffer;
    uint32_t totalLength = ringbuffer_count_get(buffer);
    int32_t seekOffset = parser->_seekOffset;
    if (seekOffset > totalLength)
    {
        return false;
    }
    int32_t j = parser->_patternMatchedCount;

    do
    {
        uint8_t vt = *(uint8_t *)ringbuffer_offset_peek_directly(buffer, seekOffset);
        if (pattern[j] == vt)
        {
            // matched
            j++;
            seekOffset++;
        }
        else
        {
            // unmatched
            if (j == 0)
            {
                // first char
                seekOffset++;
            }
            else
            {
                // not first char
                j = next[j];
            }
        }
    } while (j < patternSize && seekOffset < totalLength);
    parser->_seekOffset = seekOffset;
    parser->_patternMatchedCount = j;

    return j == patternSize;
};

/**
 * @arg
 * @arg 
 * @return 1=success, -1=dismatch, 0=not enough chars
 * */
static int8_t _packet_parser_chars_scan(PacketParser *parser, char *pattern, uint8_t size)
{
    RingBuffer *buffer = parser->buffer;
    uint32_t totalLength = ringbuffer_count_get(buffer);
    int32_t seekOffset = parser->_seekOffset;

    if ((seekOffset + size) > totalLength)
    {
        return 0;
    }
    for (uint8_t i = 0; i < size; i++)
    {
        uint32_t seekIndex = ringbuffer_offset_to_index_convert(buffer, seekOffset);
        seekOffset++;
        if (*(uint8_t *)ringbuffer_index_peek_directly(buffer, seekIndex) != pattern[i])
        {
            parser->_seekOffset = seekOffset;
            return -1;
        }
    }
    parser->_seekOffset = seekOffset;
    return 1;
};

OP_RESULT packet_parser_frame_release(PacketParser *parser, PacketFrame *frame)
{
    return ringbuffer_read_offset_sync(parser->buffer, frame->length);
};

OP_RESULT packet_parser_frame_content_extract(PacketParser *parser, PacketFrame *frame, uint8_t *buffer)
{
    OP_RESULT rst;
    rst = ringbuffer_read_offset_sync(parser->buffer, frame->contentStartOffset);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    uint32_t readed = 0;
    rst = ringbuffer_read(parser->buffer, buffer, frame->contentLength, &readed);
    if (rst != OP_RESULT_OK)
    {
        return rst;
    }
    return ringbuffer_read_offset_sync(parser->buffer, frame->length - frame->contentStartOffset - frame->contentLength);
};

OP_RESULT packet_parser_frame_extract(PacketParser *parser, PacketFrame *frame, uint8_t *buffer)
{
    uint32_t readed = 0;
    return ringbuffer_read(parser->buffer, buffer, frame->length, &readed);
};