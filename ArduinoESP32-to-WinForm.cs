using System.IO.Ports;

namespace Arduino_to_WinForm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        SerialPort SerialIO = new SerialPort(); // requires the library System.IO.Ports; installed.

        // This function runs everytime a new data is sent to the Serial Port. Ex: from the Serial.println();
        private void RecieveSerialData(object sender, SerialDataReceivedEventArgs e)
        {
            // Checks if the serial is open or not, if not then don't do anything.
            if (!SerialIO.IsOpen)
            {
                return;
            }

            try
            {
                string SerialData = SerialIO.ReadLine();  // Good for more than 1 character data.  Ex: Cat, Dog
                // String SerialData = SerialIO.Read();   // Good for only one character data. Ex: A, B

                // Insert any code here that will manipulate the SerialData.
                // Ex: string[] SerialDatas = SerialData.Split(',');

                this.Invoke((MethodInvoker)delegate
                {
                    // Insert any UI codes here
                    // Ex: label1.Text = Data (Data is the serial data) 
                });
            }

            // Just error message check which prevents any software crashes.
            catch (TimeoutException tex)
            {
                MessageBox.Show(tex.Message, "Time Out Error");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Exception Error");
            }
        }
    }
}