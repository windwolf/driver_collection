#include "../inc/common/message_parser_test.h"
#include "../inc/common/message_parser.h"

static void message_parser_test1_1()
{
    uint8_t prefix[7] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD};
    uint8_t suffix[2] = {0x0E, 0x0F};
    uint8_t buf[64] = {0};
    RingBuffer rb;
    MessageParser parser;
    MessageSchema schema = {
        .mode = MESSAGE_SCHEMA_MODE_FIXED_LENGTH,
        .fixed.fixedLength = 8,
        .prefix = prefix,
        .prefixSize = 7,
        .suffix = suffix,
        .suffixSize = 2,
    };

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test01", &schema, &rb);

    uint8_t wr0Data[3] = {0x33, 0xFA, 0xFB};
    uint8_t wr1Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x1E, 0x0F};
    uint8_t wr2Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};
    uint8_t wr3Data[13] = {0x00, 0xEF, 0xFF, 0x01, 0x02, 0x02, 0x02, 0x05, 0x06, 0x07, 0x08, 0x0E, 0x0F};

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 3, true, &aw);
    ringbuffer_write(&rb, wr1Data, 17, true, &aw);
    ringbuffer_write(&rb, wr2Data, 17, true, &aw);
    ringbuffer_write(&rb, wr3Data, 13, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;

    rst = message_parser_get(&parser, NULL, &frame);
    uint8_t fData[8];
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    rst = message_parser_get(&parser, NULL, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    uint8_t prefix2[2] = {0xEF, 0xFF};
    MessageSchema schema2 = {
        .mode = MESSAGE_SCHEMA_MODE_FIXED_LENGTH,
        .fixed.fixedLength = 8,
        .prefix = prefix2,
        .prefixSize = 2,
        .suffix = suffix,
        .suffixSize = 2,
    };
    rst = message_parser_get(&parser, &schema2, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    uint8_t wr4Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};
    ringbuffer_write(&rb, wr4Data, 17, true, &aw);

    rst = message_parser_get(&parser, NULL, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    uint8_t wr5Data[12] = {0xEF, 0xFF, 0x01, 0x02, 0x02, 0x02, 0x05, 0x06, 0x07, 0x08, 0x0E, 0x0F};
    ringbuffer_write(&rb, wr5Data, 12, true, &aw);

    rst = message_parser_get(&parser, &schema2, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }
}

static void message_parser_test1_2()
{
    uint8_t prefix[7] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD};
    uint8_t buf[64] = {0};
    RingBuffer rb;
    MessageParser parser;
    MessageSchema schema = {
        .mode = MESSAGE_SCHEMA_MODE_FIXED_LENGTH,
        .fixed.fixedLength = 8,
        .prefix = prefix,
        .prefixSize = 7,
        .suffixSize = 0,
    };

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test01", &schema, &rb);

    uint8_t wr0Data[3] = {0x33, 0xFA, 0xFB};
    uint8_t wr1Data[15] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04};
    uint8_t wr2Data[15] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04};
    uint8_t wr3Data[11] = {0x00, 0xEF, 0xFF, 0x01, 0x02, 0x02, 0x02, 0x05, 0x06, 0x07, 0x08};

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 3, true, &aw);
    ringbuffer_write(&rb, wr1Data, 15, true, &aw);
    ringbuffer_write(&rb, wr2Data, 15, true, &aw);
    ringbuffer_write(&rb, wr3Data, 11, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;

    rst = message_parser_get(&parser, NULL, &frame);
    uint8_t fData[8];
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    rst = message_parser_get(&parser, NULL, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    uint8_t prefix2[2] = {0xEF, 0xFF};
    MessageSchema schema2 = {
        .mode = MESSAGE_SCHEMA_MODE_FIXED_LENGTH,
        .fixed.fixedLength = 8,
        .prefix = prefix2,
        .prefixSize = 2,
        .suffixSize = 0,
    };
    rst = message_parser_get(&parser, &schema2, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    uint8_t wr4Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};
    ringbuffer_write(&rb, wr4Data, 17, true, &aw);

    rst = message_parser_get(&parser, NULL, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    uint8_t wr5Data[10] = {0xEF, 0xFF, 0x01, 0x02, 0x02, 0x02, 0x05, 0x06, 0x07, 0x08};
    ringbuffer_write(&rb, wr5Data, 12, true, &aw);

    rst = message_parser_get(&parser, &schema2, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }
}

static void message_parser_test2_1()
{
    uint8_t prefix[2] = {0xEF, 0xFF};
    uint8_t suffix[2] = {0x0E, 0x0F};
    MessageSchema schema = {
        .mode = MESSAGE_SCHEMA_MODE_DYNAMIC_LENGTH,
        .dynamic.lengthSize = 1,
        .prefix = prefix,
        .prefixSize = 2,
        .suffix = suffix,
        .suffixSize = 2,
    };
    RingBuffer rb;
    uint8_t buf[64] = {0};
    MessageParser parser;

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test02", &schema, &rb);

    uint8_t wr0Data[42] = {0x33, 0xFA, 0xFB,                                                              //3
                           0xEF, 0xFF, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0E, 0x0F,  //13
                           0xEF, 0xFF, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x1E, 0x0F,  //13
                           0x00, 0xEF, 0xFF, 0x01, 0x02, 0x02, 0x02, 0x05, 0x06, 0x07, 0x08, 0x0E, 0x0F}; //13

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 42, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;
    uint8_t fData[8];
    rst = message_parser_get(&parser, &schema, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    rst = message_parser_get(&parser, &schema, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    rst = message_parser_get(&parser, &schema, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }
}

static void message_parser_test2_2()
{
    uint8_t prefix[2] = {0xEF, 0xFF};
    MessageSchema schema = {
        .mode = MESSAGE_SCHEMA_MODE_DYNAMIC_LENGTH,
        .dynamic.lengthSize = 1,
        .prefix = prefix,
        .prefixSize = 2,
        .suffixSize = 0,
    };
    RingBuffer rb;
    uint8_t buf[64] = {0};
    MessageParser parser;

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test02", &schema, &rb);

    uint8_t wr0Data[42] = {0x33, 0xFA, 0xFB,                                                              //3
                           0xEF, 0xFF, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0E, 0x0F,  //13
                           0xEF, 0xFF, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x1E, 0x0F,  //13
                           0x00, 0xEF, 0xFF, 0x01, 0x02, 0x02, 0x02, 0x05, 0x06, 0x07, 0x08, 0x0E, 0x0F}; //13

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 42, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;
    uint8_t fData[8];
    rst = message_parser_get(&parser, &schema, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    rst = message_parser_get(&parser, &schema, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }

    rst = message_parser_get(&parser, &schema, &frame);
    if (rst == OP_RESULT_OK)
    {
        message_parser_frame_content_extract(&parser, &frame, fData);
    }
}

void message_parser_test()
{
    message_parser_test1_1();
    message_parser_test1_2();
    message_parser_test2_1();
    message_parser_test2_2();
};
