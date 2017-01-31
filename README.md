# esppost
esp post json 2 parameters  
actually, arduino sends AT command to server through esp8266

### test curl
```
curl -i  -H "Accept:application/json" -H "Content-Type:application/json" -XPOST "http://192.168.43.114/wsphpserver/api.php" -d '{"kode": "A100", "nama": "suzuki"}'
```
