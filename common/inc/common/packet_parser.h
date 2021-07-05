#ifndef ___PACKET_PARSER_H__
#define ___PACKET_PARSER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "shared.h"
#include "ringbuffer.h"

    typedef enum PACKET_PARSER_STAGE
    {
        PACKET_PARSE_STAGE_INIT = 0,        // schema is changed, reset everything, reparse current buffer.
        PACKET_PARSE_STAGE_PREPARING,       // Prepare to parse a new message.
        PACKET_PARSE_STAGE_SEEKING_PREFIX,  // try to seek the message's begin flags.
        PACKET_PARSE_STAGE_PARSING_LENGTH,  // try to parse the length of the message.
        PACKET_PARSE_STAGE_SEEKING_CONTENT, // try to
        PACKET_PARSE_STAGE_SEEKING_CRC,
        PACKET_PARSE_STAGE_MATCHING_SUFFIX,
        PACKET_PARSE_STAGE_SEEKING_SUFFIX,
        PACKET_PARSE_STAGE_DONE,
    } PACKET_PARSER_STAGE;

    typedef enum PACKET_SCHEMA_MODE
    {
        PACKET_SCHEMA_MODE_FIXED_LENGTH = 0,
        PACKET_SCHEMA_MODE_DYNAMIC_LENGTH,
        PACKET_SCHEMA_MODE_FREE_LENGTH,
    } PACKET_SCHEMA_MODE;

    typedef enum PACKET_SCHEMA_LENGTH_SIZE
    {
        PACKET_SCHEMA_LENGTH_SIZE_8BITS = 0,
        PACKET_SCHEMA_LENGTH_SIZE_16BITS,
        PACKET_SCHEMA_LENGTH_SIZE_24BITS,
        PACKET_SCHEMA_LENGTH_SIZE_32BITS,
    } PACKET_SCHEMA_LENGTH_SIZE;

    typedef enum PACKET_SCHEMA_CRC
    {
        PACKET_SCHEMA_CRC_NONE = 0,
    } PACKET_SCHEMA_CRC;

    /**
     * @brief 
     * fixed length mode  : | prefix  | content | (suffix) | (crc)    |
     * dynamic length mode: | prefix  | length  | content  | (suffix) | (crc)    |
     * free length mode   : | prefix  | content | (suffix) | (crc)    |
     * 
     */
    typedef struct PacketSchema
    {
        uint8_t *prefix;
        uint8_t prefixSize; // prefix size. 1-8, prefix size must not be 0.

        PACKET_SCHEMA_MODE mode;
        union
        {
            struct
            {
                uint32_t fixedLength; // the length of the content. length must not be 0.

            } fixed;
            struct
            {
                PACKET_SCHEMA_LENGTH_SIZE lengthSize; // the size of the length field.

            } dynamic;
        };
        PACKET_SCHEMA_CRC crc;
        uint8_t lengthIncludeCrc;
        uint8_t *suffix;
        uint8_t suffixSize; // suffix size. 0-8, 0 meaning that suffix is not present. if mode = free, this field must not be 0.

    } PacketSchema;

    typedef struct PacketFrame
    {
        int32_t length;
        int32_t contentStartOffset;
        int32_t contentLength;
    } PacketFrame;

    // typedef struct PacketParserContext
    // {
    //     RingBuffer *buffer;

    //     uint32_t seekOffset;

    //     uint32_t frameExpectContentLength; // The value represents content length that been parsed, if block has fixed length.
    //     uint32_t frameActualContentLength; // Frame content length that has been parsed.

    //     PacketSchema schema;

    //     PACKET_PARSER_STAGE stage;

    // } PacketParserContext;

    typedef struct PacketParser
    {
        PacketSchema *schema;
        RingBuffer *buffer;

        PacketSchema *_curSchema;
        int32_t _seekOffset; // current working seek offset. initial value is -1.
        int32_t _patternMatchedCount;

        int32_t _packetStartOffset;

        int32_t _frameExpectContentLength; // The value represents content length that been parsed, if block has fixed length.
        int32_t _frameActualContentLength; // Frame content length that has been parsed.

        PACKET_PARSER_STAGE _stage;
        int8_t _prefixPatternNexts[8];
        int8_t _suffixPatternNexts[8];
    } PacketParser;

    int8_t packet_parser_create(PacketParser *parser, char *name,
                                PacketSchema *schema,
                                RingBuffer *buffer);

    OP_RESULT packet_parser_get(PacketParser *parser, PacketSchema *customSchema, PacketFrame *parsedFrame);

    OP_RESULT packet_parser_frame_release(PacketParser *parser, PacketFrame *frame);

    OP_RESULT packet_parser_frame_content_extract(PacketParser *parser, PacketFrame *frame, uint8_t *buffer);
    OP_RESULT packet_parser_frame_extract(PacketParser *parser, PacketFrame *frame, uint8_t *buffer);
#ifdef __cplusplus
}
#endif

#endif // ___PACKET_PARSER_H__
