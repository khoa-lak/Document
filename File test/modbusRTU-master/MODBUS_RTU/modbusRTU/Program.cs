using System;
using EasyModbus;

namespace modbusRTU
{
    class Program
    {
        static void Main(string[] args)
        {
            ModbusClient modbusClient = new ModbusClient("COM1");
            modbusClient.UnitIdentifier = 1; //Not necessary since default slaveID = 1;
            modbusClient.Baudrate =19200;	// Not necessary since default baudrate = 9600
            //modbusClient.Parity = System.IO.Ports.Parity.None;
            //modbusClient.StopBits = System.IO.Ports.StopBits.Two;
            //modbusClient.ConnectionTimeout = 500;			
            modbusClient.Connect();

            modbusClient.WriteSingleCoil(4, true);      //Writes Coil #5
            modbusClient.WriteSingleRegister(19, 4711); //Writes Holding Reg. #20
            modbusClient.WriteMultipleRegisters(49, new int[10] { 112, 12, 3, 4, 53, 6, 27, 80, 9, 1 });
            modbusClient.WriteMultipleCoils(29, new bool[10] { true, true, true, true, true, true, true, true, true, true, });

            Console.WriteLine("Value of Discr. Input #1............" + modbusClient.ReadDiscreteInputs(0, 1)[0].ToString());  //Reads Discrete Input #1
            Console.WriteLine("Value of Input Reg. #10:............" + modbusClient.ReadInputRegisters(9, 1)[0].ToString());   //Reads Inp. Reg. #10
            bool[]coils5_6_7 = modbusClient.ReadCoils(0, 3);
            Console.WriteLine("Value of Coil #5-7.................." + coils5_6_7[0].ToString() + " " + coils5_6_7[1].ToString() + " " + coils5_6_7[2].ToString());   //Reads Discrete Input #5, 6, 7
            Console.WriteLine("Value of Holding Reg. #20..........." + modbusClient.ReadHoldingRegisters(19, 1)[0].ToString()); //Reads Inp. Reg. #20

            int[] regRange = modbusClient.ReadHoldingRegisters(49, 10);
            
            
            //Reads Inp. Reg. #50-59
            int idx = 0;
            foreach (var k in regRange)
            {
                Console.WriteLine("Holding Regs {0} = ..................{1}", 50 + idx,  regRange[idx].ToString()); //Reads Inp. Reg. #50-59
                idx++;
            }

            Console.Write("Press any key to continue . . . ");
            Console.ReadKey(true);
        }
    }
}
