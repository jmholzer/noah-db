/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2020, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#include "dl.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <python3.8/Python.h>

std::string get_scrape_data(const char *url)
{
    std::string file_path;
    namespace fs = std::filesystem;
    file_path = std::filesystem::current_path();
    file_path += "/dl";
    setenv("PYTHONPATH", file_path.c_str(), 0);

    Py_Initialize();

    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
    // PyObject *sys = PyImport_ImportModule("sys");
    // PyObject *path = PyObject_GetAttrString(sys, "path");
    // PyList_Append(path, PyUnicode_FromString("."));

    pName = PyUnicode_FromString("download");
    if (pName == NULL)
    {
        // std::cout << "n";
        PyErr_Print();
    }
    pModule = PyImport_Import(pName);
    // Py_DECREF(pName);
    if (pModule != NULL)
    {
        // std::cout << pModule;
        pFunc = PyObject_GetAttrString(pModule, (char *)"download");
        if (pFunc && PyCallable_Check(pFunc))
        {
            pArgs = PyTuple_Pack(1, PyUnicode_FromString((char *)url));
            pValue = PyObject_CallObject(pFunc, pArgs);
        }
        auto result = _PyUnicode_AsString(pValue);
        //std::cout << result << std::endl;
        std::string s(result);
        Py_Finalize();
        return s;
    }
    else
    {
        PyErr_Print();
        std::cout << "pModule is Null"
                  << "\n";
        EXIT_FAILURE;
    }
}

std::string save_fund_data(std::string name, const char *url)
{
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    std::string file_name = "data/" + name + "_"
                            + std::to_string(now->tm_year + 1900)
                            + "_" + std::to_string(now->tm_mon + 1)
                            + "_" + std::to_string(now->tm_mday)
                            + ".csv";
    std::ofstream myfile;
    std::string data = get_scrape_data(url);
    //std::cout << data;
    myfile.open(file_name);
    myfile << std::string(data);
    myfile.close();
    return file_name;
}

std::vector<std::string> batch_dl(std::map<std::string, std::vector<std::string>> funds)
{
    std::vector<std::string> file_names;

    for (size_t i = 0; i != funds["name"].size(); ++i)
    {
        std::string file_name = save_fund_data(funds["name"][i], funds["url"][i].c_str());
        file_names.push_back(file_name);
    }

    return file_names;
}
