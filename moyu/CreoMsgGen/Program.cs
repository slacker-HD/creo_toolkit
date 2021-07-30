using System;
using System.IO;
using System.Text;

namespace CreoMsgGen
{
    internal class Program
    {
        private const int LINELENGTH = 120;
        private const int FILELINELENGTH = 2000;

        private static String[] getstr(string strs, int len)
        {
            double i = strs.Length;
            string[] myarray = new string[int.Parse(Math.Ceiling(i / len).ToString())];
            for (int j = 0; j < myarray.Length; j++)
            {
                len = len <= strs.Length ? len : strs.Length;
                myarray[j] = strs.Substring(0, len);
                strs = strs.Substring(len, strs.Length - len);
            }
            return myarray;
        }

        private static void WriteCreofile(string filename, string[] line, int length)
        {
            FileStream fs = new FileStream(filename, FileMode.Create, FileAccess.Write);
            var utf8WithBom = new UTF8Encoding(true);
            StreamWriter sw = new StreamWriter(fs, utf8WithBom);
            for (int i = 0; i < line.Length; i++)
            {
                string firstline = String.Format("IMI{0}", i + 1);
                sw.WriteLine(firstline);
                sw.WriteLine(line[i]);
                sw.WriteLine(line[i]);
                sw.WriteLine("#");
            }

            sw.WriteLine("IMI2001");
            sw.WriteLine("已到文档最后");
            sw.WriteLine("已到文档最后");
            sw.WriteLine("#");

            string linelength = String.Format("{0}", length);
            sw.WriteLine("IMILINELENGTH");
            sw.WriteLine(linelength);
            sw.WriteLine(linelength);
            sw.WriteLine("#");

            sw.Close();
            fs.Close();
        }

        private static void Main(string[] args)
        {
            if (args == null)
            {
                Console.WriteLine("请输入要转化的文件。程序退出。");
                return;
            }

            if (args.Length != 1)
            {
                Console.WriteLine("请输入正确的参数，程序参数只有一个，为要转化的txt文件。程序退出。");
                return;
            }

            if (!File.Exists(args[0]))
            {
                Console.WriteLine("输入文件不存在，程序退出。");
                return;
            }
            string line;
            StreamReader file = new StreamReader(args[0]);
            int len = 0;
            int filelen = 1;
            string[] creoline = new string[FILELINELENGTH];
            for (int i = 0; i < FILELINELENGTH; i++)
            {
                creoline[i] = "已到文档最后";
            }
            while ((line = file.ReadLine()) != null)
            {
                if (line.Length > 0)
                {
                    if (line.Length < LINELENGTH + 1)
                    {
                        Console.WriteLine(line);
                        creoline[len] = line;
                        len++;
                        if (len == FILELINELENGTH)
                        {
                            string Creofilename = String.Format("novel_{0}.txt", filelen);
                            WriteCreofile(Creofilename, creoline, len);
                            len = 0;
                            filelen++;
                            for (int i = 0; i < FILELINELENGTH; i++)
                            {
                                creoline[i] = "已到文档最后";
                            }
                        }
                    }
                    else
                    {
                        string[] str = getstr(line, LINELENGTH);
                        foreach (string newline in str)
                        {
                            Console.WriteLine(newline);
                            creoline[len] = newline;
                            len++;
                            if (len == FILELINELENGTH)
                            {
                                string Creofilename = String.Format("novel_{0}.txt", filelen);
                                WriteCreofile(Creofilename, creoline, len);
                                len = 0;
                                filelen++;
                                for (int i = 0; i < FILELINELENGTH; i++)
                                {
                                    creoline[i] = "已到文档最后";
                                }
                            }
                        }
                    }
                }
            }
            if (len > 0)
            {
                string Creofilename = String.Format("novel_{0}.txt", filelen);
                WriteCreofile(Creofilename, creoline, len);
            }
            file.Close();
            Console.WriteLine("转化完毕");
        }
    }
}