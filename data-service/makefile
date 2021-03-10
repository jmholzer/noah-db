ETL: ETL.cpp ETL_sql.cpp
	g++ ETL.cpp ETL_sql.cpp -lsqlite3 -o ETL

curl_compile: 
	g++ curl/curl.cpp -l curl -o curl/curl.exe
curl_exe:
	./curl/curl.exe

curl: curl_compile curl_exe

all: curl ETL 