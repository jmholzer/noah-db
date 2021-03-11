ETL: ETL.cpp ETL_sql.cpp
	g++ ETL.cpp ETL_sql.cpp -lsqlite3 -o ETL

curl_compile: 
	g++-10 -std=c++17 curl/curl.cpp -lcurl -o curl/curl.exe
curl_exe:
	./curl/curl.exe

acurl_compile: 
	g++-10 -std=c++17 curl/acurl.cpp -lcurl -o curl/acurl.exe 
acurl_exe:
	./curl/acurl.exe

curl: curl_compile curl_exe

acurl: acurl_compile acurl_exe

all: curl ETL 