#ifndef ___MESSAGE_PARSER_H__
#define ___MESSAGE_PARSER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "shared.h"
#include "ringbuffer.h"

    typedef enum MESSAGE_PARSER_STAGE
    {
        MESSAGE_PARSE_STAGE_INIT = 0,        // schema is changed, reset everything, reparse current buffer.
        MESSAGE_PARSE_STAGE_PREPARING,       // Prepare to parse a new message.
        MESSAGE_PARSE_STAGE_SEEKING_PREFIX,  // try to seek the message's begin flags.
        MESSAGE_PARSE_STAGE_PARSING_LENGTH,  // try to parse the length of the message.
        MESSAGE_PARSE_STAGE_SEEKING_CONTENT, // try to
        MESSAGE_PARSE_STAGE_SEEKING_CRC,
        MESSAGE_PARSE_STAGE_MATCHING_SUFFIX,
        MESSAGE_PARSE_STAGE_SEEKING_SUFFIX,
        MESSAGE_PARSE_STAGE_DONE,
    } MESSAGE_PARSER_STAGE;

    typedef enum MESSAGE_SCHEMA_MODE
    {
        MESSAGE_SCHEMA_MODE_FIXED_LENGTH = 0,
        MESSAGE_SCHEMA_MODE_DYNAMIC_LENGTH,
        MESSAGE_SCHEMA_MODE_FREE_LENGTH,
    } MESSAGE_SCHEMA_MODE;

    typedef enum MESSAGE_SCHEMA_LENGTH_SIZE
    {
        MESSAGE_SCHEMA_LENGTH_SIZE_8BITS = 1,
        MESSAGE_SCHEMA_LENGTH_SIZE_16BITS,
        MESSAGE_SCHEMA_LENGTH_SIZE_24BITS,
        MESSAGE_SCHEMA_LENGTH_SIZE_32BITS,
    } MESSAGE_SCHEMA_LENGTH_SIZE;

    typedef enum MESSAGE_SCHEMA_LENGTH_ENDIAN
    {
        MESSAGE_SCHEMA_LENGTH_ENDIAN_LITTLE = 0,
        MESSAGE_SCHEMA_LENGTH_ENDIAN_BIG,
    } MESSAGE_SCHEMA_LENGTH_ENDIAN;

    typedef enum MESSAGE_SCHEMA_CRC
    {
        MESSAGE_SCHEMA_CRC_NONE = 0,
    } MESSAGE_SCHEMA_CRC;

    /**
     * @brief 
     * fixed length mode  : | prefix  | content | (suffix) | (crc)    |
     * dynamic length mode: | prefix  | length  | content  | (suffix) | (crc)    |
     * free length mode   : | prefix  | content | (suffix) | (crc)    |
     * 
     */
    typedef struct MessageSchema
    {
        uint8_t *prefix;
        uint8_t prefixSize; // prefix size. 1-8, prefix size must not be 0.

        MESSAGE_SCHEMA_MODE mode;
        union
        {
            struct
            {
                uint32_t fixedLength; // the length of the content. length must not be 0.

            } fixed;
            struct
            {
                MESSAGE_SCHEMA_LENGTH_SIZE lengthSize; // the size of the length field.
                MESSAGE_SCHEMA_LENGTH_ENDIAN endian;

            } dynamic;
        };
        MESSAGE_SCHEMA_CRC crc;
        uint8_t lengthIncludeCrc;
        uint8_t *suffix;
        uint8_t suffixSize; // suffix size. 0-8, 0 meaning that suffix is not present. if mode = free, this field must not be 0.

    } MessageSchema;

    typedef struct MessageFrame
    {
        int32_t length;
        int32_t contentStartOffset;
        int32_t contentLength;
    } MessageFrame;

    // typedef struct PacketParserContext
    // {
    //     RingBuffer *buffer;

    //     uint32_t seekOffset;

    //     uint32_t frameExpectContentLength; // The value represents content length that been parsed, if block has fixed length.
    //     uint32_t frameActualContentLength; // Frame content length that has been parsed.

    //     MessageSchema schema;

    //     MESSAGE_PARSER_STAGE stage;

    // } PacketParserContext;

    typedef struct MessageParser
    {
        MessageSchema *schema;
        RingBuffer *buffer;

        MessageSchema *_curSchema;
        int32_t _seekOffset; // current working seek offset. initial value is -1.
        int32_t _patternMatchedCount;

        int32_t _packetStartOffset;

        int32_t _frameExpectContentLength; // The value represents content length that been parsed, if block has fixed length.
        int32_t _frameActualContentLength; // Frame content length that has been parsed.

        MESSAGE_PARSER_STAGE _stage;
        int8_t _prefixPatternNexts[8];
        int8_t _suffixPatternNexts[8];
    } MessageParser;

    int8_t message_parser_create(MessageParser *parser, char *name,
                                 MessageSchema *schema,
                                 RingBuffer *buffer);

    OP_RESULT message_parser_get(MessageParser *parser, MessageSchema *customSchema, MessageFrame *parsedFrame);

    OP_RESULT message_parser_frame_release(MessageParser *parser, MessageFrame *frame);

    OP_RESULT message_parser_frame_content_extract(MessageParser *parser, MessageFrame *frame, uint8_t *buffer);

    OP_RESULT message_parser_frame_extract(MessageParser *parser, MessageFrame *frame, uint8_t *buffer);

#ifdef __cplusplus
}
#endif

#endif // ___MESSAGE_PARSER_H__
