using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CsharpDll
{
    public class MyDialog
    {
        public static int ShowWindow(string Name)
        {
            MyForm form = new MyForm(Name);
            form.Show();
            return 1;
        }
    }
}
