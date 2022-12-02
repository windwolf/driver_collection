// #include "uart_stream_demo.hpp"
// #include "soc.hpp"
// #include "accessor/uart_stream.hpp"
// #include "message/message_parser.hpp"

// //#include "communication_protocol/inc/tree_accessor.h"
// //#include "console/inc/console.h"

// #include "os.hpp"

// namespace wibot::device::demo
// {

// using namespace wibot::peripheral;
// using namespace wibot::accessor;
// using namespace wibot::device;
// using namespace wibot::os;
// using namespace wibot::comm;

// extern UART_HandleTypeDef huart4;
// static RAM2_BUFFER UART uart(huart4);
// static RAM2_BUFFER uint8_t txBuf0[64];
// static RAM2_BUFFER uint8_t uartRxBuffer[64];
// static RAM2_BUFFER RingBuffer uartRxRingBuffer(uartRxBuffer, 1, 64);
// static RAM2_BUFFER UartStream stream(uart, uartRxRingBuffer);
// static MessageParser mp(uartRxRingBuffer);
// static MessageSchema schema = {
//     .prefixSize = 0,
//     .mode = MESSAGE_SCHEMA_MODE_FREE_LENGTH,

//     .crc =
//             {
//                 .length = MESSAGE_SCHEMA_SIZE_NONE,
//             },
//     .suffix = {'\r', '\n'},
//     .suffixSize = 2,
// };
// // static TreeAccessor ta;
// // static Console console;
// static int valueA;

// // static bool getter(const void *context, const int32_t index, char *out)
// // {
// //     sprintf(out, "%d", valueA);
// // };

// // static bool setter(const void *context, const int32_t index, char *in)
// // {
// //     scanf(in, "%d", &valueA);
// // };

// // static TreeAccessorItemEntry itemA = {
// //     .name = "a",
// //     .getter = getter,
// //     .setter = setter,
// // };

// static int32_t cWrite = 0;
// static int32_t cRead = 0;

// static RAM2_DATA const char *text = "test wolf!";

// #define THREAD4_STACK_SIZE 4096
// static uint8_t thread4_stack[THREAD4_STACK_SIZE];
// static Thread thread_4;

// static void thread_4_entry(ULONG thread_input);

// void uart_stream_demo()
// {
//     ringbuffer_create(&uartRxRingBuffer, uartRxBuffer, 1, 64);
//     uart_device_create(&uartDevice, &huart4, 0);
//     stream_create(&stream, &uartDevice, &uartRxRingBuffer);
//     message_parser_create(&mp, "demo", &schema, &uartRxRingBuffer);
//     tree_accessor_create(&ta, NULL, NULL, NULL);
//     tree_accessor_item_register(&ta, "/", &itemA);
//     console_create(&console, &stream, &ta, &mp);

//     tx_thread_create(&thread_4, "thread 1", thread_4_entry, 1, thread4_stack,
//                      THREAD4_STACK_SIZE, 16, 16, 4, TX_AUTO_START);
// };

// static void thread_4_entry(ULONG thread_input)
// {
//     console_start(&console);

//     // UINT status;
//     // UartDevice_Tx(&uart, (uint8_t *)text, strlen(text));
//     // UartDevice_WaitForTxComplete(&uart, TX_WAIT_FOREVER);
//     // stream_send(&stream, (uint8_t *)text, strlen(text));
//     // stream_send_cplt_wait(&stream, TX_WAIT_FOREVER);

//     // while (1)
//     // {

//     //     stream_receive_ready_wait(&stream, TX_WAIT_FOREVER);
//     //     uint32_t len = ringbuffer_count_get(&uartRxRingBuffer);
//     //     if (len > 0)
//     //     {
//     //         uint32_t actualLength = 0;
//     //         ringbuffer_read(&uartRxRingBuffer, txBuf0, len, &actualLength);
//     //         stream_send(&stream, txBuf0, actualLength);
//     //         //w25qxx_write(&w25qxx_1, txBuf0, 0x0400, len);
//     //         //w25qxx_read(&w25qxx_1, data_buf, 0x0400, len);
//     //         tx_thread_sleep(10);
//     //         //cRead++;
//     //     }
//     // }
// }
// } // namespace wibot::device::demo
