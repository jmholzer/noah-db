CREATE TABLE "holdings" (
	"fund_id"	TEXT,
	"ticker"	TEXT,
	"exchange"	TEXT,
	"quantity"	REAL,
	"unix_time"	INTEGER,
	PRIMARY KEY("fund_id","ticker","exchange","unix_time")
);
