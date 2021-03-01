CREATE TABLE funds (
    fund_id TEXT,
    fund_name TEXT,
    fund_manager TEXT,
    PRIMARY KEY(fund_id)
    FOREIGN KEY(fund_id) REFERENCES holdings(fund_id)
);

CREATE TABLE holdings (
    fund_id TEXT,
    ticker TEXT,
    exchange TEXT,
    quantity REAL,
    unix_time INTEGER,
    PRIMARY KEY(fund_id, ticker, exchange, unix_time),
    FOREIGN KEY(ticker) REFERENCES shares(ticker)
    FOREIGN KEY(exchange) REFERENCES shares(exchange)
    FOREIGN KEY(ticker) REFERENCES share_prices(ticker)
    FOREIGN KEY(exchange) REFERENCES share_prices(exchange)
);

CREATE TABLE shares (
    ticker TEXT,
    exchange TEXT,
    company_name TEXT,
    PRIMARY KEY(ticker, exchange),
    FOREIGN KEY(ticker) REFERENCES share_prices(ticker)
    FOREIGN KEY(exchange) REFERENCES share_prices(exchange)
);

CREATE TABLE share_prices (
    ticker TEXT,
    exchange TEXT,
    price REAL,
    unix_time INTEGER,
    PRIMARY KEY(ticker, exchange, unix_time)
    FOREIGN KEY(ticker) REFERENCES shares(ticker)
    FOREIGN KEY(exchange) REFERENCES shares(exchange)
);