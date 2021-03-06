#!/bin/sh


# $(cat /proc/cpuinfo | grep Revision | awk '{print $3}')


cheie="zzzzzzzzzzzzzzzzz"


 tt=$(cat /proc/cpuinfo | grep Revision | awk '{print $3}')
 vv=$(echo $tt | tr '[a-z]' '[A-Z]')
 raspi_ver=$(echo "obase=10; "$vv"" | /usr/bin/bc) 

# variabila_pres="1"
# variabila_pres=$(/daisypi/bmp085/pres1)
# variabila_sht=$(/daisypi/sht11/sht11_move)
#variabila_sht=$(/daisypi/sht11/sht_read_17CLK_22_SDA)
# variabila_sht_sec=$(/daisypi/sht11/sht11_secund)
# variabila_adc=$(/daisypi/mcp3304/adc_1.py)
# variab_cpu=$(cat /sys/class/thermal/thermal_zone0/temp)
# variabila_analog=$(python /daisypi/mcp3208/adc_read.py)

# echo " VV $variab_cpu"
# read_temp=$(echo $variabila_sht | awk '{print $2}')
# read_humid=$(echo $variabila_sht | awk '{print $3}')
# read_dew=$(echo $variabila_sht | awk '{print $4}')

# read_temp_sec=$(echo $variabila_sht_sec | awk '{print $2}')
# read_humid_sec=$(echo $variabila_sht_sec | awk '{print $3}')
# read_dew_sec=$(echo $variabila_sht_sec | awk '{print $4}')

# cpu_temp=$(echo "scale=2; "$variab_cpu"/1000" | /usr/bin/bc)
# ilum=$(nice -19 "/daisypi/tsl235r/tsl_read" | awk '{print $4}')
# ilum=$(echo $ilum | awk '{print $4}')
# echo "BP  "$variabila_pres
# Version check. Greater than 3 mean second version, then revision is given in hex and can exceed 14.

if [ "$raspi_ver" -gt 3 ]; then 
 echo "VERSION 2"
else
  
 echo "VERSION 1"
fi




feed_id="38780"
curl -s 'http://api.sen.se/feeds/'$feed_id'/last_event/?sense_key='$cheie -X GET -H "Content-type: application/json"
exit 0