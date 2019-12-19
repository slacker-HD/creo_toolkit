using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace CsharpDll
{
    public partial class MyForm : Form
    {
        [DllImport("ToolkitCsharp.dll", EntryPoint = "SaveFile", ExactSpelling = false, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SaveFile();

        public MyForm(String Name)
        {
            InitializeComponent();
            labeltxt.Text  = "传入了数据：" + Name;
        }

        public MyForm()
        {
            InitializeComponent();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SaveFile();
        }
    }
}