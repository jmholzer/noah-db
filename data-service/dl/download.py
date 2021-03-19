import cfscrape

def download(url):
    scraper = cfscrape.create_scraper()
    data = scraper.get(url).text
    return data

#if __name__ == "__main__":
#    download("https://ark-funds.com/wp-content/fundsiteliterature/csv/ARK_FINTECH_INNOVATION_ETF_ARKF_HOLDINGS.csv")
