using System;
using System;
using HtmlAgilityPack;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading;
namespace ConsoleApp5
{
    internal class Program
    {
        static string DecodeName(byte[] bytes, int len, int id)
        {
            // MemorySerializeEncrypted from Proton SDK
            string key = "PBG892FXX982ABC*";
            string result = "";
            for (int i = 0; i < len; i++)
            {
                result += (char)(bytes[i] ^ key[(i + id) % 16]);
            }
            return result;
        }
       static List<string> itemList = new List<string>();
        static void DecodeItemsDat()
        {
            Stream stream = new FileStream("items.dat", FileMode.Open);

            byte[] useForIntReading = new byte[4];
            byte[] useForShortReading = new byte[4];

            int itemCount = 0;
            short unused = 0;

            stream.Read(useForShortReading, 0, 2);
            unused = BitConverter.ToInt16(useForShortReading, 0);

            stream.Read(useForIntReading, 0, 4);
            itemCount = BitConverter.ToInt32(useForIntReading, 0);


            for (int c = 0; c < itemCount; c++)
            {
                short itemID = 0;
                stream.Read(useForShortReading, 0, 2);
                itemID = BitConverter.ToInt16(useForShortReading, 0);
                stream.Seek(2, SeekOrigin.Current);
                byte editableType = (byte)stream.ReadByte();
                byte editableCategory = (byte)stream.ReadByte();
                byte actionType = (byte)stream.ReadByte();
                byte hitSound = (byte)stream.ReadByte();

                short itemNameLength;
                stream.Read(useForShortReading, 0, 2);
                itemNameLength = BitConverter.ToInt16(useForShortReading, 0);

                byte[] itemNameEncoded = new byte[itemNameLength];
                stream.Read(itemNameEncoded, 0, itemNameLength);
                string decodedItemName = DecodeName(itemNameEncoded, itemNameLength, itemID);

                short fileNameLength;
                stream.Read(useForShortReading, 0, 2);
                fileNameLength = BitConverter.ToInt16(useForShortReading, 0);

                byte[] filenameBytes = new byte[fileNameLength];
                stream.Read(filenameBytes, 0, fileNameLength);
                string filename = Encoding.ASCII.GetString(filenameBytes);

                byte[] texturehash_ = new byte[4];
                stream.Read(texturehash_, 0, 4);
                int texturehash = BitConverter.ToInt32(texturehash_, 0);

                byte itemKind = (byte)stream.ReadByte();
                stream.Seek(4, SeekOrigin.Current);
                byte textureX = (byte)stream.ReadByte();
                byte textureY = (byte)stream.ReadByte();
                byte spreadType = (byte)stream.ReadByte();
                byte isStripey = (byte)stream.ReadByte();
                byte collType = (byte)stream.ReadByte();
                byte hitsToDestroy = (byte)stream.ReadByte();
                byte dropChance = (byte)stream.ReadByte();
                int clothingType = 0;
                // No
                stream.Seek(3, SeekOrigin.Current);
                clothingType = stream.ReadByte();
                short rarity = 0;
                stream.Read(useForShortReading, 0, 2);
                rarity = BitConverter.ToInt16(useForShortReading, 0);
                byte toolKind = (byte)stream.ReadByte();
                short audioFileLength = 0;
                stream.Read(useForShortReading, 0, 2);
                audioFileLength = BitConverter.ToInt16(useForShortReading, 0);

                byte[] audioFileBytes = new byte[audioFileLength];
                stream.Read(audioFileBytes, 0, audioFileLength);
                string audioFileStr69420 = Encoding.ASCII.GetString(audioFileBytes);

                byte[] audioHash_ = new byte[4];
                stream.Read(audioHash_, 0, 4);
                int audioHash = BitConverter.ToInt32(audioHash_, 0);

                short audioVolume = 0;
                stream.Read(useForShortReading, 0, 2);
                audioVolume = BitConverter.ToInt16(useForShortReading, 0);

                string petName = "";
                string petPrefix = "";
                string petSuffix = "";
                string petAbility = "";
                string extraFieldUnk5 = "";

                stream.Read(useForShortReading, 0, 2);
                short extraFieldLenCommon = BitConverter.ToInt16(useForShortReading, 0);
                if (extraFieldLenCommon < 30 && extraFieldLenCommon >= 0)
                {
                    byte[] bytesToWriteToStr = new byte[extraFieldLenCommon];
                    stream.Read(bytesToWriteToStr, 0, extraFieldLenCommon);
                    petName = Encoding.ASCII.GetString(bytesToWriteToStr);
                }
                stream.Read(useForShortReading, 0, 2);
                extraFieldLenCommon = BitConverter.ToInt16(useForShortReading, 0);
                if (extraFieldLenCommon < 30 && extraFieldLenCommon >= 0)
                {
                    byte[] bytesToWriteToStr = new byte[extraFieldLenCommon];
                    stream.Read(bytesToWriteToStr, 0, extraFieldLenCommon);
                    petPrefix = Encoding.ASCII.GetString(bytesToWriteToStr);
                }
                stream.Read(useForShortReading, 0, 2);
                extraFieldLenCommon = BitConverter.ToInt16(useForShortReading, 0);
                if (extraFieldLenCommon < 30 && extraFieldLenCommon >= 0)
                {
                    byte[] bytesToWriteToStr = new byte[extraFieldLenCommon];
                    stream.Read(bytesToWriteToStr, 0, extraFieldLenCommon);
                    petSuffix = Encoding.ASCII.GetString(bytesToWriteToStr);
                }
                stream.Read(useForShortReading, 0, 2);
                extraFieldLenCommon = BitConverter.ToInt16(useForShortReading, 0);
                if (extraFieldLenCommon < 30 && extraFieldLenCommon >= 0)
                {
                    byte[] bytesToWriteToStr = new byte[extraFieldLenCommon];
                    stream.Read(bytesToWriteToStr, 0, extraFieldLenCommon);
                    petAbility = Encoding.ASCII.GetString(bytesToWriteToStr);
                }
                stream.Read(useForShortReading, 0, 2);
                extraFieldLenCommon = BitConverter.ToInt16(useForShortReading, 0);
                if (extraFieldLenCommon < 30 && extraFieldLenCommon >= 0)
                {
                    byte[] bytesToWriteToStr = new byte[extraFieldLenCommon];
                    stream.Read(bytesToWriteToStr, 0, extraFieldLenCommon);
                    extraFieldUnk5 = Encoding.ASCII.GetString(bytesToWriteToStr);
                }
                // This should do the job of stream.Seek(+10)!

                byte seedBase = (byte)stream.ReadByte();
                byte seedOverlay = (byte)stream.ReadByte();
                byte treeBase = (byte)stream.ReadByte();
                byte treeLeaves = (byte)stream.ReadByte();

                // Color is ARGB.
                byte[] color1 = new byte[4];
                stream.Read(color1, 0, 4);
                byte[] color2 = new byte[4];
                stream.Read(color2, 0, 4);

                short ingredient1 = 0;
                stream.Read(useForShortReading, 0, 2);
                ingredient1 = BitConverter.ToInt16(useForShortReading, 0);
                short ingredient2 = 0;
                stream.Read(useForShortReading, 0, 2);
                ingredient2 = BitConverter.ToInt16(useForShortReading, 0);

                int growTimeSec = 0;
                stream.Read(useForIntReading, 0, 4);
                growTimeSec = BitConverter.ToInt16(useForIntReading, 0);

                short extraFieldShort3 = 0;
                stream.Read(useForShortReading, 0, 2);
                extraFieldShort3 = BitConverter.ToInt16(useForShortReading, 0);

                short isRayman = 0;
                stream.Read(useForShortReading, 0, 2);
                isRayman = BitConverter.ToInt16(useForShortReading, 0);

                short extraFieldLength = 0;
                stream.Read(useForShortReading, 0, 2);
                extraFieldLength = BitConverter.ToInt16(useForShortReading, 0);
                string extraOptions = "";
                byte[] bytesToWriteToStr234 = new byte[extraFieldLength];
                stream.Read(bytesToWriteToStr234, 0, extraFieldLength);
                extraOptions = Encoding.ASCII.GetString(bytesToWriteToStr234);


                short extraField2Length = 0;
                stream.Read(useForShortReading, 0, 2);
                extraField2Length = BitConverter.ToInt16(useForShortReading, 0);
                string extraFilename = "";
                bytesToWriteToStr234 = new byte[extraField2Length];
                stream.Read(bytesToWriteToStr234, 0, extraField2Length);
                extraFilename = Encoding.ASCII.GetString(bytesToWriteToStr234);

                short extraField3Length = 0;
                stream.Read(useForShortReading, 0, 2);
                extraField3Length = BitConverter.ToInt16(useForShortReading, 0);
                string extraOptions2 = "";
                bytesToWriteToStr234 = new byte[extraField3Length];
                stream.Read(bytesToWriteToStr234, 0, extraField3Length);
                extraOptions2 = Encoding.ASCII.GetString(bytesToWriteToStr234);

                short extraField4Length = 0;
                stream.Read(useForShortReading, 0, 2);
                extraField4Length = BitConverter.ToInt16(useForShortReading, 0);
                string extraFieldUnk_4 = "";
                bytesToWriteToStr234 = new byte[extraField4Length];
                stream.Read(bytesToWriteToStr234, 0, extraField4Length);
                extraFieldUnk_4 = Encoding.ASCII.GetString(bytesToWriteToStr234);

                stream.Seek(4, SeekOrigin.Current);
                short value = 0;
                stream.Read(useForShortReading, 0, 2);
                value = BitConverter.ToInt16(useForShortReading, 0);
                short value2 = 0;
                stream.Read(useForShortReading, 0, 2);
                value2 = BitConverter.ToInt16(useForShortReading, 0);

                short unkValueShort1 = 0;
                stream.Read(useForShortReading, 0, 2);
                unkValueShort1 = BitConverter.ToInt16(useForShortReading, 0);

                stream.Seek(16 - value, SeekOrigin.Current);

                short unkValueShort2 = 0;
                stream.Read(useForShortReading, 0, 2);
                unkValueShort2 = BitConverter.ToInt16(useForShortReading, 0);

                // we're done parsing, skip a bunch of bytes
                stream.Seek(50, SeekOrigin.Current);

                short extraField5Length = 0;
                stream.Read(useForShortReading, 0, 2);
                extraField5Length = BitConverter.ToInt16(useForShortReading, 0);
                string extraFieldUnk_5 = "";
                bytesToWriteToStr234 = new byte[extraField5Length];
                stream.Read(bytesToWriteToStr234, 0, extraField5Length);
                extraFieldUnk_5 = Encoding.ASCII.GetString(bytesToWriteToStr234);

                // New Version
                if (unused > 11)
                {
                    stream.Seek(21, SeekOrigin.Current);
                }

                // add the item info to the file
                string audiofilestring = audioFileStr69420;
                string file_ = $"{decodedItemName}\r\n";
                itemList.Add(decodedItemName);
                Console.Write($"\rProcessing items {(int)((float)((float)(c + 1) / (float)itemCount) * 100.0f)}%   ");
            }
            Console.Write("\n");
        }
       static int getItemIDFromItemName(string itemname)
        {
            for (int i = 0; i < itemList.Count(); i++)
            {
                if (itemList[i] == itemname)
                {
                    return i;
                    break;
                }
            }
            return 0;
        }
        static void RequestItemVars(string itemname,int itemid)
        {
            bool found = false;
            string Chi = "None";
            string Splicing = "";
            string Splicing1 = "";
            string WearPlaymod = "";
            string UnWearPlaymod = "";
            string Description = "";
            string IgNore = "";
            string playmod = "";

            HtmlWeb web = new HtmlWeb();
            HtmlDocument doc = web.Load("https://growtopia.fandom.com/wiki/" + itemname);
            Console.WriteLine("Loaded the item " + itemname);
            string d = doc.DocumentNode.InnerHtml;
            if (d.Contains("card-text"))
            {
                IEnumerable<HtmlNode> nodes =
                    doc.DocumentNode.Descendants(0)
                        .Where(n => n.HasClass("card-text"));

                Description = nodes.ElementAt(0).InnerText;
                Console.WriteLine("Description: {0}", nodes.ElementAt(0).InnerText + "|");
            }
            // filter html elements on the basis of class name
            List<string> list = new List<string>();
            using (StringReader reader = new StringReader(d))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    if (line.Contains("&#160;&#160;&#160;") && line.Contains("CheckboxEnabled.png") && line.StartsWith("<td style="))
                    {
                        string[] split = line.Split(new string[] { "&#160;&#160;&#160;" }, StringSplitOptions.None);
                        list.Add(split[1]);
                    }
                    if (line.Contains("&#160;&#160;&#160;") && line.Contains("Checkbox0.png") && line.StartsWith("<td style="))
                    {
                        string[] split = line.Split(new string[] { "&#160;&#160;&#160;" }, StringSplitOptions.None);
                        list.Add(split[1]);

                    }

                    if (line.Contains("<p>When equipped,") && line.Contains("<i>"))
                    {
                        var docrs = new HtmlAgilityPack.HtmlDocument();
                        docrs.LoadHtml(line);

                        foreach (HtmlNode node in docrs.DocumentNode.SelectNodes("//i"))
                        {
                            string value = node.InnerText;
                            playmod = value;
                            Console.WriteLine("PlayMod: {0}", value);
                        }
                    }
                    if (line.Contains("<td><span class=\"seed growsprite\">") && !found)
                    {
                        string tableTag = line.Replace("span ", "");
                        var docrs = new HtmlAgilityPack.HtmlDocument();
                        docrs.LoadHtml(tableTag);

                        IEnumerable<HtmlNode> nodes = docrs.DocumentNode.Descendants(0);
                        //nodes.ElementAt(i).InnerText

                        if (d.Contains("Splicing") && d.Contains("https://static.wikia.nocookie.net/growtopia/images/e/e5/TreeSprites.png/revision/latest/window-crop/width/32/x-offset/192/y-offset/896/window-width/32/window-height/32?format=webp&amp;fill=cb-20220923025746"))
                        {
                            Console.WriteLine("Splicing Seed Found at this Item!");
                            string txt = nodes.ElementAt(0).InnerText;
                            //Console.WriteLine(txt);
                            Splicing = nodes.ElementAt(4).InnerText;
                            Splicing1 = nodes.ElementAt(nodes.Count()-1).InnerText;
                            Console.WriteLine("Splicing Seed 1: {0}\nSplicing Seed 2: {1}", Splicing , Splicing1);
                        }
                        else
                        {
                            Console.WriteLine("Splicnig Seed 1: {0}\nSplicnig Seed 2: {1}", "0", "0");
                        }
                        found = true;
                    }

                }
            }
            if (d.Contains("CheckboxEnabled.png") && d.Contains("Function") && d.Contains("Checkbox0.png"))
            {
                try
                {
                    WearPlaymod = list[0];
                    UnWearPlaymod = list[1];

                    Console.WriteLine("when item wears the first playmod: {0}\nwhen item wears the first playmod 2: {1}", list[0], list[1]);
                }
                catch (Exception asd)
                {
                    Console.WriteLine("when item wears the first playmod: {0}\nwhen item wears the first playmod 2: {1}", "error", "error");
                    WearPlaymod = "";
                    UnWearPlaymod = "";

                }
            }
            else
            {
                Console.WriteLine("when item wears the first playmod: {0}\nwhen item wears the first playmod 2: {1}", "Cant Wearable", "Cant Wearable");
                WearPlaymod = "";
                UnWearPlaymod = "";

            }
            var p = doc.DocumentNode.SelectNodes("//p");
            var td = doc.DocumentNode.SelectNodes("//td");//span
            int gems = 0;

            Console.WriteLine("IGNORETHISPART: " + p[0].InnerText.Trim());
            IgNore = p[0].InnerText.Trim();
            if (d.Contains("Data"))
            {
                Console.WriteLine("Chi: " + td[1].InnerText.Trim());

                Chi = td[1].InnerText.Trim();
                if(!d.Contains("</span> N/A</td>"))
                {
                    Console.WriteLine("Max Defaults Gem Drop: " + td[7].InnerText.Trim());
                    gems = Convert.ToInt32(td[7].InnerText.Trim().Split('-')[1]);
                    Console.WriteLine("Max Gems: " + gems);
                }
            }
            else
            {
                Console.WriteLine("Chi: Not Found");
                Chi = "None";
                Console.WriteLine("Max Defaults Gem Drop: Not Found");

            }
            string seed = getItemIDFromItemName(Splicing).ToString();
            string seed1 = getItemIDFromItemName(Splicing1).ToString();
            if (seed == "0") seed = "";
            if (seed1 == "0") seed1 = "";
            if (Chi == "Earth") Chi = "0";
            else if (Chi == "Fire") Chi = "1";
            else if (Chi == "Wind") Chi = "2";
            else if (Chi == "Water") Chi = "3";
            else Chi = "4";
            string playm = playmod.Replace("&amp;", "&");
            string desc = Description.Replace("&#160;", " ");
            Console.WriteLine(itemid.ToString());
            string SaveToDatabase = String.Format("{0}|{1}|{2}|{3}|{4}|{5}|{6}|{7}|{8}|", itemid.ToString(), Chi, seed, seed1, WearPlaymod, UnWearPlaymod, desc.Replace("\n", " "), gems.ToString(), playm.Replace("&nbsp;", ""));
            File.AppendAllText("wiki.txt", SaveToDatabase.Replace("’", "'") + "\n");
        }
        static void Main(string[] args)
        {
            DecodeItemsDat();
            Console.Title = "Wiki Parser made by SrMotion#7900";
            Console.WriteLine("NOTE: If you have left your process unfinished, delete the text file you made in the past.");

             //RequestItemVars("Magic_Egg", 610);
            for (int i = 0; i < itemList.Count; i++)
            {
                if (i % 2 == 0)
                {
                    if (getItemIDFromItemName(itemList[i]).ToString().Contains("null_"))
                    {
                        Console.WriteLine("Null Item Detected!");
                    }
                    else
                    {
                        RequestItemVars(itemList[i], i);
                        System.Threading.Thread.Sleep(100);
                        Console.WriteLine("----------------------------");
                    }
                }
                else
                {
                    Console.WriteLine("Seed Detected id: " + i);
                }
                Console.WriteLine(i);
            }
            Console.ReadKey();
        }
    }
}
