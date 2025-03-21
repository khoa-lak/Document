/*
   Created by ArduinoGetStarted.com

   This example code is in the public domain

   Tutorial page: https://arduinogetstarted.com/tutorials/arduino-write-variable-to-sd-card
*/
#include <SPI.h>
#include <SD.h>

//#define PIN_SPI_CS 4

File myFile;
int myInt = -52;
float myFloat = -12.7;
String myString = "HELLO";
char myCharArray[] = "ArduinoGetStarted.com";
byte myByteArray[] = {'1', '2', '3', '4', '5'};
//char data[] =  ":020000040800F2"
//               ":1020000070060020892100080B2B0008912A000887"
//               ":10201000092B00088B220008672B00080000000035"
//               ":102020000000000000000000000000000F2B00086E"
//               ":102030008D220008000000000D2B0008112B000865"
//               ":10204000A3210008A3210008A3210008A321000860"
//               ":10205000A3210008A3210008A3210008A321000850"
//               ":10206000A3210008A3210008A3210008A321000840"
//               ":10207000A3210008A3210008A3210008A321000830"
//               ":10208000A3210008A3210008A3210008A321000820"
//               ":10209000A3210008A3210008A3210008A321000810"
//               ":1020A000A3210008A3210008A3210008A321000800"
//               ":1020B000A3210008A3210008A3210008A3210008F0"
//               ":1020C000A3210008A3210008A3210008A3210008E0"
//               ":1020D000A3210008A3210008A3210008A3210008D0"
//               ":1020E000A3210008A3210008A321000800F002F8A2"
//               ":1020F00000F03AF80AA090E8000C82448344AAF168"
//               ":102100000107DA4501D100F02FF8AFF2090EBAE865"
//               ":102110000F0013F0010F18BFFB1A43F0010318471B"
//               ":10212000B80A0000D80A0000103A24BF78C878C165"
//               ":10213000FAD8520724BF30C830C144BF04680C60CD"
//               ":10214000704700000023002400250026103A28BF15"
//               ":1021500078C1FBD8520728BF30C148BF0B60704719"
//               ":102160001FB51FBD10B510BD00F058F81146FFF7A0"
//               ":10217000F7FF00F009FD00F076F803B4FFF7F2FF77"
//               ":1021800003BC00F07BF8000009488047094800477D"
//               ":10219000FEE7FEE7FEE7FEE7FEE7FEE7FEE7FEE717"
//               ":1021A000FEE7FEE704480549054A064B7047000074"
//               ":1021B000652B0008ED20000870000020700600204C"
//               ":1021C00070020020700200204FF0000200B513469C"
//               ":1021D00094469646203922BFA0E80C50A0E80C5047"
//               ":1021E000B1F12001BFF4F7AF090728BFA0E80C50F8"
//               ":1021F00048BF0CC05DF804EB890028BF40F8042BF1"
//               ":1022000008BF704748BF20F8022B11F0804F18BF5D"
//               ":1022100000F8012B7047704770477047754600F013"
//               ":1022200029F8AE4605006946534620F0070085466A"
//               ":1022300018B020B5FFF7B6FFBDE820404FF000060C"
//               ":102240004FF000074FF000084FF0000B21F007019E"
//               ":10225000AC46ACE8C009ACE8C009ACE8C009ACE8E1"
//               ":10226000C0098D4670470446AFF300802046FFF753"
//               ":1022700084FF00000048704710000020014918202A"
//               ":10228000ABBEFEE7260002007047FEE77047000085"
//               ":1022900070B5044600F00CF90546601C02D00448F5"
//               ":1022A0000078044400F004F9401BA042FAD370BD4A"
//               ":1022B000000000202DE9F84FDFF8B8910024A9F1C3"
//               ":1022C000040B23460F2709F1040AABF1040EC9E001"
//               ":1022D0000125A5402A40AA427DD1674ED1F804C00D"
//               ":1022E000ACEB0608B44532D014DCBCF1030F3AD095"
//               ":1022F00009DCBCF1000F2AD0BCF1010F1CD0BCF1ED"
//               ":10230000020F31D11DE0BCF1110F17D0BCF1120F3B"
//               ":102310002AD119E0B8F5881F19D006DCB8F5803F3E"
//               ":1023200015D0B8F5801F1FD111E0B8F5001F0ED0F1"
//               ":10233000B8F5041F18D10AE0CB6815E0CB681B1D67"
//               ":1023400012E0CB6808330FE0CB680C330CE08B68ED"
//               ":102350002BB1012B4FF0080303D0456104E00423A7"
//               ":1023600002E0056100E00023FF2A01D8064601E0F3"
//               ":1023700000F104064FEA840501D9A5F12005D6F83D"
//               ":10238000008007FA05FC28EA0C0803FA05F548EA7C"
//               ":102390000508C6F800804D68ED0062D5374DAE697E"
//               ":1023A00046F00106AE61AD6924F0030605F00105B3"
//               ":1023B0000095334D06EB050CDCF80850A607360FE8"
//               ":1023C00007FA06F825EA0805DFF8B880404503D18A"
//               ":1023D0004FF0000817E044E0DFF8AC80404502D140"
//               ":1023E0004FF001080FE0DFF8A480404502D14FF024"
//               ":1023F000020808E0DFF89880404502D14FF003085A"
//               ":1024000001E04FF0040808FA06F848EA0508CCF89D"
//               ":1024100008804D68D9F80060ED024D4601D516439D"
//               ":1024200000E096432E604D68DAF80060AD02554634"
//               ":1024300001D5164300E096432E604D68DBF800603E"
//               ":10244000AD035D4601D5164300E096432E604D680E"
//               ":10245000DEF80060ED03754601D5164300E09643B3"
//               ":102460002E60641C0D682A46E5407FF431AFBDE85C"
//               ":10247000F88F000008040140000011100010024015"
//               ":102480000000014000080140000C01400010014024"
//               ":1024900000140140C26821EA02030A4043EA0241F3"
//               ":1024A000016170470AB1016170470904FBE7000050"
//               ":1024B00001488068704700000000002002488168E1"
//               ":1024C000027811448160704700000020074810B571"
//               ":1024D000016841F010010160032000F067F80F204F"
//               ":1024E00000F006F800F024F8002010BD00200240A3"
//               ":1024F00070B50D4D04464FF47A712878B1FBF0F0B9"
//               ":102500000A490968B1FBF0F000F0AEFA48B9102CA6"
//               ":1025100007D200222146501E00F028F800206C60EF"
//               ":1025200070BD012070BD0000000000200C000020E4"
//               ":102530000C4808B5816941F001018161816901F0B0"
//               ":1025400001010091C16941F08051C161C06900F091"
//               ":10255000805000900448416821F0E06141F0007132"
//               ":10256000416008BD0010024000000140F0B40E4B75"
//               ":102570001B680C46C3F30223C3F10705042D00D9E1"
//               ":102580000425191D072901D2002300E0DB1E0126C6"
//               ":1025900006FA05F1491E214099409E40761E1640DC"
//               ":1025A0003143F0BC00F0E0BA0CED00E0064900F069"
//               ":1025B000070208684FF6FF03184040EA0220034A6A"
//               ":1025C00010430860704700000CED00E00000FA05C1"
//               ":1025D0002DE9F0470F46050061D0434E306800F00A"
//               ":1025E0000700B84209D2306820F007003843306055"
//               ":1025F000306800F00700B84251D128683B4C810791"
//               ":1026000012D5400703D5606840F4E0606060287828"
//               ":10261000000703D5606840F4605060606068A96896"
//               ":1026200020F0F000084360602878C00728D0686870"
//               ":10263000012810D02168022810D0890700292EDA3D"
//               ":10264000616821F0030101436160FFF731FF8046BB"
//               ":1026500041F288390DE021688903EFE78901EDE750"
//               ":10266000FFF726FFA0EB0801494502D90320BDE88A"
//               ":10267000F0876068696800F00C00B0EB810FEFD163"
//               ":10268000306800F00700B8420BD9306820F007002E"
//               ":1026900038433060306800F00700B84201D00120B4"
//               ":1026A000E5E72878400705D56068E96820F4E06030"
//               ":1026B000084360602878000706D56068296920F41F"
//               ":1026C000605040EAC100606000F018F86168084A94"
//               ":1026D000C1F30311515CC840064908600648006810"
//               ":1026E000FFF706FF0020C2E7002002400010024072"
//               ":1026F000C62B00080C000020040000200E4A516880"
//               ":102700000E4801F00C03042B11D0082B0FD10C48FC"
//               ":10271000C1F38343C903C05C0AD55168084AC1F3B9"
//               ":102720004041921E515C054A5043B0FBF1F07047A6"
//               ":1027300004494843704700000010024000127A002C"
//               ":10274000B62B000800093D002DE9F84F040070D0B9"
//               ":102750002078C04DC00749D06868C0F381000128C7"
//               ":1027600007D06868C0F38100022809D16868C003F7"
//               ":1027700006D52868800339D560680028E7D035E0A1"
//               ":102780006068B0F5803F0BD010B1B0F5A02F11D02C"
//               ":10279000286820F480302860286820F4802002E037"
//               ":1027A000286840F480302860606890B1FFF780FEB0"
//               ":1027B00006460AE0286840F480202860F0E700BF61"
//               ":1027C000FFF776FE801B642873D828688003F7D54E"
//               ":1027D0000CE0FFF76DFE064605E000BFFFF768FE60"
//               ":1027E000801B6428F0D828688003F7D420789A4E9C"
//               ":1027F00080074FF001094FF0000839D5686810F0E4"
//               ":102800000C0F07D06868C0F38100022809D16868FE"
//               ":10281000C00306D42868800714D52069012808D190"
//               ":1028200010E02069B0B1C6F80090FFF741FE0746FE"
//               ":1028300005E00AE1FFF73CFEC01B0228C4D8286867"
//               ":102840008007F7D52868616920F0F80040EAC100E8"
//               ":1028500028600DE0C6F80080FFF72AFE074604E076"
//               ":10286000FFF726FEC01B022823D828688007F7D46C"
//               ":10287000207800072CD57948A169D9B1C0F800901B"
//               ":10288000FFF716FE074604E0FFF712FEC01B022802"
//               ":102890000FD8686A8007F7D571484FF4FA5100687D"
//               ":1028A000B0FBF1F0009000BF0098411E0091FAD2F9"
//               ":1028B0000EE080E0C0F80080FFF7FAFD074604E074"
//               ":1028C000FFF7F6FDC01B022875D8686A8007F7D4A9"
//               ":1028D0002078400760D5E8690027C00008D4E8697F"
//               ":1028E00040F08050E861E869012700F080500090D6"
//               ":1028F0005C480168C90513D40168834641F48071BE"
//               ":102900000160FFF7D5FD824606E000BFFFF7D0FD6E"
//               ":10291000A0EB0A0064284ED8DBF80000C005F5D50E"
//               ":10292000E06801280AD008B1052813D0286A20F0F1"
//               ":1029300001002862286A20F0040002E0286A40F0C2"
//               ":1029400001002862E06841F2883B98B1FFF7B0FDD2"
//               ":1029500082460BE0286A40F004002862EEE700BFE0"
//               ":10296000FFF7A6FDA0EB0A0159455AD8286A80074F"
//               ":10297000F6D50CE0FFF79CFD824605E0FFF798FDD9"
//               ":10298000A0EB0A0159454CD8286A8007F6D41FB13C"
//               ":10299000E86920F08050E861E06988B36968C1F3B4"
//               ":1029A0008101022944D00228C6F8608004D0FFF7D4"
//               ":1029B0007FFD044638E034E0FFF77AFD074604E087"
//               ":1029C000FFF776FDC01B02282BD828688001F7D4BA"
//               ":1029D000206AB0F5803F05D16868A16820F4003016"
//               ":1029E00008436860D4E908010843696821F4741158"
//               ":1029F00008436860C6F86090FFF75AFD044606E099"
//               ":102A000025E000BFFFF754FD001B022809D8286805"
//               ":102A10008001F7D51BE000BFFFF74AFD001B02282D"
//               ":102A200002D90320BDE8F88F28688001F4D40EE0B5"
//               ":102A30000128F7D06868226A00F48031914204D1FD"
//               ":102A4000616A00F47010884201D00120EAE700209A"
//               ":102A5000E8E70000001002400000424280044242C9"
//               ":102A60000C0000200070004010B5401EB0F1807FC7"
//               ":102A700001D3012010BD4FF0E02460610F216017E9"
//               ":102A800000F072F80020A06107202061002010BD36"
//               ":102A9000FEE7000070B586B0002418480094019449"
//               ":102AA00002940394816941F010018161816901F010"
//               ":102AB00010010491816941F0040181618069104E27"
//               ":102AC00000F004004FF400550490002229463046DF"
//               ":102AD000FFF7E8FC0220CDE9024000950125694698"
//               ":102AE00001953046FFF7E6FBCDE9005469460548FD"
//               ":102AF0000295FFF7DFFB06B070BD0000001002403A"
//               ":102B00000010014000080140FEE7FEE770477047F3"
//               ":102B1000FFF7D4BC10B590B028216846FFF754FBEE"
//               ":102B200014210AA8FFF750FB022000900120049016"
//               ":102B300010200024059007946846FFF705FE08B1B1"
//               ":102B400072B6FEE70F20CDE90A040C940D94002123"
//               ":102B50000AA80E94FFF73CFD002801D072B6FEE7EC"
//               ":102B600010B010BD7047FEE70907090E002804DB0E"
//               ":102B700000F1E02080F80014704700F00F0000F131"
//               ":102B8000E02080F8141D7047FFF7A0FCFFF7C2FF9C"
//               ":102B9000FFF780FF064D4FF4005440F2DC5621460B"
//               ":102BA0002846FFF777FC3046FFF772FBF7E7000097"
//               ":102BB00000100140010202030405060708090A0B80"
//               ":102BC0000C0D0E0F101000000000000000000102AC"
//               ":102BD0000304060708090000F82B00080000002085"
//               ":102BE0001000000028210008082C00081000002018"
//               ":102BF00060060000442100080100000010000000F1"
//               ":082C00000000000000127A0040"
//               ":04000005080020EDE2"
//               ":00000001FF";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  SPI.begin();
  if (!SD.begin(PA4)) {
    Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
    while (1); // don't do anything more:
  }

  Serial.println(F("SD CARD INITIALIZED."));
  Serial.println(F("--------------------"));
  SD.remove("Logspond.txt"); // delete the file if existed
  //SD.rename("temp.txt", fileName);
  // create new file by opening file for writing
//  myFile = SD.open("FileData.txt", FILE_WRITE);
//
//  if (myFile) {
//    myFile.println(data);    // write int variable to SD card in line
//    myFile.close();
//  }
//  else {
//    Serial.print(F("SD Card: error on opening file arduino.txt"));
//  }
  //
  //  // open file for reading
  //  myFile = SD.open("FileData.txt", FILE_READ);
  //  char file[9000] = {'0'};
  //  uint16_t i = 0;
  //  if (myFile) {
  //    while (myFile.available()) {
  //      file[i] = (char)myFile.read(); // read characters one by one from Micro SD Card
  //      //
  //      //file[i] = ch; // print the character to Serial Monitor
  //      i++;
  //      //Serial.print(ch);
  //    }
  //    Serial.println(String(strlen(file)));
  //    Serial.println(file);
  //    //    Serial.println("------------------");
  //    //    Serial.println(myFile.position());
  //    //    myFile.seek(5);
  //    //    Serial.println(myFile.position());
  //    //    myFile.write(' ');
  //    //    myFile.seek(6);
  //    //    Serial.println(myFile.position());
  //    //    myFile.write(' ');
  //    //    Serial.println("------------------");
  //    //    myFile.seek(0);
  //    //    while (myFile.available()) {
  //    //      char ch = myFile.read(); // read characters one by one from Micro SD Card
  //    //      Serial.print(ch); // print the character to Serial Monitor
  //    //    }
  //    myFile.close();
  //  }
  //  else {
  //    Serial.print(F("SD Card: error on opening file arduino.txt"));
  //  }
}

void loop() {
  //  char txt1[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupid tat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  //  char txt2[] = "...Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  //  uint32_t soLan = 82;
  //  while (1) {
  //    uint8_t numDong = 1;
  //    myFile = SD.open("arduino.txt", FILE_WRITE);
  //    Serial.println("size của file là: " + String(myFile.size()));
  //    if (myFile) {
  //      char at[100];
  //      sprintf(at, "lần thứ: %d, text số: %d", soLan, numDong);
  //      myFile.print(at);
  //      myFile.println(txt1);
  //      numDong++;
  //      sprintf(at, "lần thứ: %d, text số: %d", soLan, numDong);
  //      myFile.print(at);
  //      myFile.println(txt2);
  //      soLan++;
  //      Serial.println(at);
  //      myFile.close();
  //    } else {
  //      Serial.println("fail");
  //    }
  //    delay(1);
  //  }
}
