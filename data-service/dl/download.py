import cfscrape

def download(url):
    scraper = cfscrape.create_scraper()
    data = scraper.get(url).text
    return data

# if __name__ == "__main__":
#     print('fddf')
    # download("test")