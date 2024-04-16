using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
//using EasyModbus;
//using ModbusRTU;
using System.Collections;
namespace test485
{
    public partial class Form1 : Form
    {
        //ModbusClient modbusClient;
        //modbusrtu _Modbusrtu;
        public Form1()
        {
            InitializeComponent();

        }

       

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.PortName = "COM12";
            serialPort1.Open();
            //modbusrtu.Instance().Open("COM1", 19200,Parity.Even);
            // modbusClient = new ModbusClient("COM1");
            //modbusClient.UnitIdentifier = 1;
            //modbusClient.Baudrate = 19200;	// Not necessary since default baudrate = 9600
            //modbusClient.Parity = Parity.Even;
            // modbusClient.StopBits = StopBits.One;
            //modbusClient.ConnectionTimeout =200;
            // modbusClient.Connect();

        }
        int ae = 1;
        int[] coils5_6_7 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        short[] k = new short[250];
        short[] k1 = new short[250];
        string chuoi = "U{\"device_id\":\"water_quality_f9s7dga09sdfas0df\",\"message_type\": \"settings\",\"timestamp\": 1634534678,\"data\": {\"command\": \"on\",\"properties\": {\"interval\": 24,\"qty\": 4,\"params\":[\"PH\",\"NO2\",\"NH3\",\"O2\"]}}}";
        private void btnOff_Click(object sender, EventArgs e)
        {
            byte[] buffer = new byte[1024];
            int bytesRead = serialPort1.Read(buffer, 0, buffer.Length);

            string data = Encoding.ASCII.GetString(buffer, 0, bytesRead);
            textBox2.Text = bytesRead.ToString();
            //for (int i = 0; i < 250; i++)
            //  {
            //      if (i < chuoi.Length) {
            //          k[i] = (short)chuoi[i];                  
            //      }
            //      else
            //      {
            //          k[i] = 0;             
            //      }
            //      int value = k[i];
            //      //modbusClient.WriteSingleRegister(0,value);
            //  }
            // textBox2.Text = string.Join(" ",k);
            //modbusrtu.Instance().FC06(1, 0, 145);
        }
        private void btnOn_Click(object sender, EventArgs e)
        {
             textBox1.Text = "";
            serialPort1.Write(chuoi);
            /* for (int i = 0; i < 10; i++)
             {
                 textBox1.Text += modbusClient.ReadHoldingRegisters(0, 1)[0].ToString();
                 textBox1.Text += " ";

             }*/
            /* int[] regRange = new int[250];
              for(int i =0; i < 250; i++)
             {

              int[] tr = modbusClient.ReadHoldingRegisters(1, 10);
             regRange[0] = tr[0];
             }


            textBox1.Text = string.Join(" ", regRange);*/

            //modbusrtu.Instance().FC03(1,0,10,ref k1);
            //for (int i = 0; i < 0; i++)
            //{
            //    textBox1.Text += k1[i].ToString() + " ";
            //}
            //textBox1.Text += k1[0].ToString() + " ";
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            serialPort1.Close();
        }
    }
}
