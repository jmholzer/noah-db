#ETL: ETL.cpp 
	#g++-10 -std=c++17 ETL.cpp dl/dl.cpp db/db.cpp csv/csv.cpp -lsqlite3 -lcurl -L/usr/lib/x86_64-linux-gnu -lpython3.6m -g -o ETL.out

ETL: ETL.cpp
	g++-10 -std=c++17 ETL.cpp dl/dl.cpp db/db.cpp csv/csv.cpp -lsqlite3 -lcurl -L/usr/lib/x86_64-linux-gnu $$(python3-config --cflags) $$(python3-config --ldflags) -lpython3.8 -fPIE -g -o ETL.out
	
requests_test_compile:
	g++-10 -std=c++17 requests_test.cpp cURL/requests.cpp -lcurl -o request_test.out

requests_test_run:
	./request_test.out
