#include "../inc/common/message_parser_test.h"
#include "../inc/common/message_parser.h"
#include "minunit.h"
static const uint8_t refData[8] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04};
static void message_parser_test1_1()
{

    uint8_t prefix[7] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD};
    uint8_t suffix[2] = {0x0E, 0x0F};
    uint8_t buf[64] = {0};
    RingBuffer rb;
    MessageParser parser;
    MessageSchema schema = {
        .mode = MESSAGE_SCHEMA_MODE_FIXED_LENGTH,
        .fixed.length = 8,
        .prefix = prefix,
        .prefixSize = 7,
        .suffix = suffix,
        .suffixSize = 2,
        .crc.length = MESSAGE_SCHEMA_LENGTH_SIZE_NONE,
    };

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test01", &schema, &rb);

    uint8_t wr0Data[3] = {0x33, 0xFA, 0xFB};
    uint8_t wr1Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x1E, 0x0F};
    uint8_t wr2Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};
    uint8_t wr3Data[13] = {0x00, 0xEF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 3, true, &aw);
    ringbuffer_write(&rb, wr1Data, 17, true, &aw);
    ringbuffer_write(&rb, wr2Data, 17, true, &aw);
    ringbuffer_write(&rb, wr3Data, 13, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;

    // test1_1:1
    rst = message_parser_frame_get(&parser, NULL, &frame);
    MU_ASSERT("test1_1:1 X", rst != OP_RESULT_OK);
    uint8_t fData[8];
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_1:1r X", fData, refData, 8);
    }

    // test1_1:2
    rst = message_parser_frame_get(&parser, NULL, &frame);
    MU_ASSERT("test1_1:2 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_1:2r X", fData, refData, 8);
    }

    uint8_t prefix2[2] = {0xEF, 0xFF};
    MessageSchema schema2 = {
        .mode = MESSAGE_SCHEMA_MODE_FIXED_LENGTH,
        .fixed.length = 8,
        .prefix = prefix2,
        .prefixSize = 2,
        .suffix = suffix,
        .suffixSize = 2,
        .crc.length = MESSAGE_SCHEMA_LENGTH_SIZE_NONE,
    };

    // test1_1:3
    rst = message_parser_frame_get(&parser, &schema2, &frame);
    MU_ASSERT("test1_1:3 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_1:3r X", fData, refData, 8);
    }

    uint8_t wr4Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};
    ringbuffer_write(&rb, wr4Data, 17, true, &aw);

    // test1_1:4
    rst = message_parser_frame_get(&parser, NULL, &frame);
    MU_ASSERT("test1_1:4 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_1:4r X", fData, refData, 8);
    }

    uint8_t wr5Data[12] = {0xEF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};
    ringbuffer_write(&rb, wr5Data, 12, true, &aw);

    // test1_1:5
    rst = message_parser_frame_get(&parser, &schema2, &frame);
    MU_ASSERT("test1_1:5 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_1:5r X", fData, refData, 8);
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
        .fixed.length = 8,
        .prefix = prefix,
        .prefixSize = 7,
        .suffixSize = 0,
        .crc.length = MESSAGE_SCHEMA_LENGTH_SIZE_NONE,
    };

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test01", &schema, &rb);

    uint8_t wr0Data[3] = {0x33, 0xFA, 0xFB};
    uint8_t wr1Data[15] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04};
    uint8_t wr2Data[15] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04};
    uint8_t wr3Data[11] = {0x00, 0xEF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04};

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 3, true, &aw);
    ringbuffer_write(&rb, wr1Data, 15, true, &aw);
    ringbuffer_write(&rb, wr2Data, 15, true, &aw);
    ringbuffer_write(&rb, wr3Data, 11, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;
    // test1_2:1
    rst = message_parser_frame_get(&parser, NULL, &frame); //1
    MU_ASSERT("test1_2:1 X", rst == OP_RESULT_OK);
    uint8_t fData[8];
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_2:1r X", fData, refData, 8);
    }

    rst = message_parser_frame_get(&parser, NULL, &frame); //1
    MU_ASSERT("test1_2:2 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_2:2r X", fData, refData, 8);
    }

    uint8_t prefix2[2] = {0xEF, 0xFF};
    MessageSchema schema2 = {
        .mode = MESSAGE_SCHEMA_MODE_FIXED_LENGTH,
        .fixed.length = 8,
        .prefix = prefix2,
        .prefixSize = 2,
        .suffixSize = 0,
        .crc.length = MESSAGE_SCHEMA_LENGTH_SIZE_NONE,
    };
    rst = message_parser_frame_get(&parser, &schema2, &frame); //1
    MU_ASSERT("test1_2:3 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_2:3r X", fData, refData, 8);
    }

    uint8_t wr4Data[17] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFA, 0xFB, 0xFD, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F};
    ringbuffer_write(&rb, wr4Data, 17, true, &aw);

    rst = message_parser_frame_get(&parser, NULL, &frame); //1
    MU_ASSERT("test1_2:4 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_2:4r X", fData, refData, 8);
    }

    uint8_t wr5Data[10] = {0xEF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04}; //1
    ringbuffer_write(&rb, wr5Data, 12, true, &aw);

    rst = message_parser_frame_get(&parser, &schema2, &frame); //1
    MU_ASSERT("test1_2:5 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test1_2:5r X", fData, refData, 8);
    }
}

static void message_parser_test2_1()
{
    uint8_t prefix[2] = {0xEF, 0xFF};
    uint8_t suffix[2] = {0x0E, 0x0F};
    MessageSchema schema = {
        .mode = MESSAGE_SCHEMA_MODE_DYNAMIC_LENGTH,
        .dynamic.range = MESSAGE_SCHEMA_RANGE_CONTENT,
        .dynamic.lengthSize = 1,
        .prefix = prefix,
        .prefixSize = 2,
        .suffix = suffix,
        .suffixSize = 2,
        .crc.length = MESSAGE_SCHEMA_LENGTH_SIZE_NONE,
    };
    RingBuffer rb;
    uint8_t buf[64] = {0};
    MessageParser parser;

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test02", &schema, &rb);

    uint8_t wr0Data[43] = {0x33, 0xFA, 0xFB,                                                                    //3
                           0xEF, 0xFF, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F,        //13
                           0xEF, 0xFF, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x1E, 0x0F,        //13
                           0x00, 0xEF, 0xFF, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F}; //14

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 43, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;
    uint8_t fData[8];
    rst = message_parser_frame_get(&parser, &schema, &frame);
    MU_ASSERT("test2_1:1 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test2_1:1r X", fData, refData, 8);
    }

    rst = message_parser_frame_get(&parser, &schema, &frame);
    MU_ASSERT("test2_1:2 X", rst != OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test2_1:2r X", fData, refData, 8);
    }

    rst = message_parser_frame_get(&parser, &schema, &frame);
    MU_ASSERT("test2_1:3 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test2_1:3r X", fData, refData, 8);
    }
}

static void message_parser_test2_2()
{
    uint8_t prefix[2] = {0xEF, 0xFF};
    MessageSchema schema = {
        .mode = MESSAGE_SCHEMA_MODE_DYNAMIC_LENGTH,
        .dynamic.range = MESSAGE_SCHEMA_RANGE_CONTENT,
        .dynamic.lengthSize = 1,
        .prefix = prefix,
        .prefixSize = 2,
        .suffixSize = 0,
        .crc.length = MESSAGE_SCHEMA_LENGTH_SIZE_NONE,
    };
    RingBuffer rb;
    uint8_t buf[64] = {0};
    MessageParser parser;

    ringbuffer_create(&rb, buf, 1, 64);

    message_parser_create(&parser, "test02", &schema, &rb);

    uint8_t wr0Data[42] = {0x33, 0xFA, 0xFB,                                                              //3
                           0xEF, 0xFF, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F,  //13
                           0xEF, 0xFF, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x1E, 0x0F,  //13
                           0x00, 0xEF, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x03, 0x04, 0x0E, 0x0F}; //13

    uint32_t aw;
    ringbuffer_write(&rb, wr0Data, 42, true, &aw);

    MessageFrame frame;
    OP_RESULT rst;
    uint8_t fData[8];
    rst = message_parser_frame_get(&parser, &schema, &frame);
    MU_ASSERT("test2_2:1 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test2_2:1r X", fData, refData, 8);
    }

    rst = message_parser_frame_get(&parser, &schema, &frame);
    MU_ASSERT("test2_2:2 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test2_2:2r X", fData, refData, 8);
    }

    rst = message_parser_frame_get(&parser, &schema, &frame);
    MU_ASSERT("test2_2:3 X", rst == OP_RESULT_OK);
    if (rst == OP_RESULT_OK)
    {
        MU_VEC_CLEAR(fData, 8);
        message_parser_frame_content_extract(&frame, fData);
        MU_ASSERT_VEC_EQUALS("test2_2:3r X", fData, refData, 8);
    }
}

void message_parser_test()
{
    message_parser_test1_1();
    message_parser_test1_2();
    message_parser_test2_1();
    message_parser_test2_2();
};
