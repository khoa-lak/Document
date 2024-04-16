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
using System.IO;

namespace Test232
{
    public partial class Form1 : Form
    {
        string InputData = String.Empty; // Khai báo string buff dùng cho hiển thị dữ liệu sau này.
        delegate void SetTextCallback(string text); // Khai bao delegate SetTextCallBack voi tham so string
        public Form1()
        {
            InitializeComponent();

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            cbPort.DataSource = SerialPort.GetPortNames();
            btnGet.Enabled = false;
            btnSend.Enabled = false;
            btnConnect.BackColor = Color.Red;
            btnDis.BackColor = Color.Green;
            timer1.Enabled = false;
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(serialPort1_DataReceived);
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = cbPort.Text;
                //erialPort1.ReadTimeout = 2000;          
                serialPort1.BaudRate = 115200;
                serialPort1.Open();
                
            

                btnConnect.BackColor = Color.Green;
                btnDis.BackColor = Color.Red;
                btnGet.Enabled = true;
                btnSend.Enabled = true;
                timer1.Enabled =true;
            }
        }

        private void btnDis_Click(object sender, EventArgs e)
        {

            serialPort1.Close();
            btnConnect.BackColor = Color.Red;
            btnDis.BackColor = Color.Green;
            btnGet.Enabled = false;
            btnSend.Enabled = false;
            timer1.Enabled = false;
        }

        private void btnSend_Click(object sender, EventArgs e)
        {    
            txtSend.Text = "";
            txtGet.Text = "";
             string json = "\"qwertyuiopasdfghjkl\"";
            //string json =" \"{\"device_id\":\"water_quality_f9s7dga09sdfas0df\",\"message_type\": \"settings\",\"timestamp\":1634534678,\"data\": {\"command\": \"on\",\"properties\": {\"interval\":24,\"qty\": 4,\"params\":[\"PH\",\"NO2\",\"NH3\",\"O2\"]}}}\"";
            //string json ="{\"device_id\":\"water_quality_f9s7dga09sdfas0df\",\"message_type\": \"settings\",\"timestamp\":1634534678,\"data\": {\"command\": \"on\",\"properties\": {\"interval\":24,\"qty\": 4,\"params\":[\"PH\",\"NO2\",\"NH3\",\"O2\"]}}}";
            //char[] input = json.ToCharArray();
            serialPort1.Write(json);
            //if (serialPort1.BytesToRead > 0)
            //{
            //    txtSend.Text += serialPort1.ReadExisting();
            //}
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            txtGet.Text = "";
            string getMes = serialPort1.ReadLine();
            txtGet.Text = getMes;
            string json = "{\"device_id\":\"water_quality_f9s7dga09sdfas0df\",\"message_type\": \"settings\",\"timestamp\":1634534678,\"data\": {\"command\": \"on\",\"properties\": {\"interval\":24,\"qty\": 4,\"params\":[\"PH\",\"NO2\",\"NH3\",\"O2\"]}}}";
            serialPort1.WriteLine(json);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            serialPort1.Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

            
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            InputData = serialPort1.ReadExisting();
            if (InputData != String.Empty)
            {
                // txtIn.Text = InputData; // Ko dùng đc như thế này vì khác threads .
                SetText(InputData); // Chính vì vậy phải sử dụng ủy quyền tại đây. Gọi delegate đã khai báo trước đó.
            }
        }
        private void SetText(string text)
        {
           

            if (txtGet.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText); // khởi tạo 1 delegate mới gọi đến SetText
                this.Invoke(d, new object[] { text });
            }
            else txtGet.Text += text;
        }
    }
}
