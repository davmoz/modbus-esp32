#ifndef MYLIB_CONSTANTS_H
#define MYLIB_CONSTANTS_H

namespace ModbusFunctionCodes
{
    const int READ_HOLDING_REGISTERS = 0x03;     // Read the contents of read/write location(4X references)
    const int READ_INPUT_REGISTERS = 0x03;       // Read the contents of read only location(3X references)
    const int DIAGNOSTICS = 0x08;                // Only sub-function zero is supported. This returns the data element of the query unchanged.
    const int PRE_SET_MULTIPLE_REGISTERS = 0x0F; // Set the contents of read/write location (4X references)
}

namespace MySQLConstants
{
    const int DBPoolSize = 0xFF; // Just some example

}
#endif
