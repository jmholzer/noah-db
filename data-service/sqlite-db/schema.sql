CREATE TABLE funds (
    fund_id INTEGER PRIMARY KEY,
    fund_ticker TEXT,
    fund_name TEXT,
    fund_manager TEXT
);

CREATE TABLE holdings (
    fund_id INTEGER,
    share_id INTEGER,
    quantity REAL,
    unix_time INTEGER,
    PRIMARY KEY(fund_id, share_id, unix_time),
    FOREIGN KEY(fund_id) REFERENCES shares(share_id),
    FOREIGN KEY(fund_id) REFERENCES prices(share_id)
);

CREATE TABLE shares (
    share_id INTEGER,
    company_name TEXT,
    ticker TEXT,
    exchange TEXT,
    PRIMARY KEY(share_id),
    FOREIGN KEY(share_id) REFERENCES prices(share_id)
);

CREATE TABLE share_prices (
    share_id INTEGER PRIMARY KEY,
    price REAL,
    unix_time INTEGER,
    FOREIGN KEY(share_id) REFERENCES shares(share_id)
);