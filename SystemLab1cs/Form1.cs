using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;

namespace SystemLab1cs
{
    public partial class Form1 : Form
    {
        Process ConsoleApp = null;
        EventWaitHandle stopEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StopEvent");
        EventWaitHandle startEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        EventWaitHandle confirmEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        EventWaitHandle closeEvent = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseEvent");
        public Form1()
        {
            InitializeComponent();
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (ConsoleApp == null || ConsoleApp.HasExited)
            {
                ConsoleApp = Process.Start("SystemLab1cpp.exe");
                listThreads.Items.Clear();
                listThreads.Items.Add("All Threads:");
                listThreads.Items.Add("Main Thread");
            }
            else
            {
                for (int i = 1; i <= numericUpDown1.Value; i++)
                {
                    startEvent.Set();
                    confirmEvent.WaitOne();
                    listThreads.Items.Add("Thread " + (listThreads.Items.Count-1));
                }
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (ConsoleApp != null && !ConsoleApp.HasExited)
            {
                stopEvent.Set();
                confirmEvent.WaitOne();
                if (listThreads.Items.Count > 2)
                    listThreads.Items.RemoveAt(listThreads.Items.Count-1);
                else if(listThreads.Items.Count == 2)
                {
                    ConsoleApp.CloseMainWindow();
                    listThreads.Items.RemoveAt(listThreads.Items.Count-1);
                }
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            closeEvent.Set();
            return;
        }

        private void Form1_Activated(object sender, EventArgs e)
        {
            if (ConsoleApp == null || ConsoleApp.HasExited)
            {
                listThreads.Items.Clear();
                listThreads.Items.Add("All Threads:");
            }
        }
    }
}
