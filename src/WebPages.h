const char CONFIG_PAGE[] PROGMEM = R"=====(
<HTML>
 <HEAD>
   <TITLE>Battery WiFi Bridge</TITLE>
 </HEAD>
 <BODY>
  <form action="/settings" method="get">
   %s<br><br>
   <br><br>
   <input type="submit" value="Save" style='width: 150px;'>
   &nbsp;&nbsp;&nbsp;
   <a href="/reboot">
    <button type="button" style='width: 150px;'>Restart</button>
   </a>
  </form>
 </BODY>
</HTML>
)=====";

const char GET_JSON[] PROGMEM = R"=====({
 "voltage":%.2f,
 "current":%.2f,
 "power":%.0f,
 "cell1_v": %.3f,
 "cell2_v": %.3f,
 "cell3_v": %.3f,
 "cell4_v": %.3f,
 "cell5_v": %.3f,
 "cell6_v": %.3f,
 "cell7_v": %.3f,
 "cell8_v": %.3f,
 "cell9_v": %.3f,
 "cell10_v": %.3f,
 "cell11_v": %.3f,
 "cell12_v": %.3f,
 "cell13_v": %.3f,
 "cell14_v": %.3f,
 "cell15_v": %.3f,
 "cell16_v": %.3f,
 "temp1": %.1f,
 "temp2": %.1f,
 "temp3": %.1f,
 "temp4": %.1f,
 "env_temp": %1.f,
 "bms_temp": %1.f,
 "soc": %.1f,
 "rssi":%d
})=====";
