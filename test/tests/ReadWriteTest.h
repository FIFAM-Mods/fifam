#pragma once
#include "FifamReadWrite.h"

class ReadWriteTest {
public:
    ReadWriteTest() {
        FifamWriter writer(L"test.sav", 7, 0x2007, 0xC);
        writer.WriteVersion();
        writer.WriteLine(L"Test");
        writer.WriteLine(Quoted(L"Quoted"));
        writer.WriteLine(Quoted(L"Quoted 1"), Quoted(L"Quoted 2"), Quoted(L"Quoted 3"));
        writer.WriteLine(L"One", L"Two", L"Three");
        writer.WriteLine(1, 2.0f, 3.0);
        writer.WriteLine(Hexadecimal(0xAAA));
        writer.WriteLine(FifamDate(26, 6, 2018));
        writer.WriteLineWithSeparator('|', L"First name", L"Second name");
        std::vector<int> intAry = { 1, 2, 3, 4, 5 };
        writer.WriteLineArray(intAry);
        std::vector<std::wstring> strAry = { L"a", L"b", L"c", L"d", L"e" };
        writer.WriteLineArray(strAry);
        writer.Close();

        FifamReader reader(L"test.sav", 7);
        reader.ReadVersion();
        std::wcout << L"version:" << reader.GetVersion().GetString() << std::endl;
        std::wcout << reader.ReadLine<std::wstring>() << std::endl;
        std::wcout << reader.ReadLine<Quoted>()() << std::endl;
        std::wstring q1, q2, q3;
        reader.ReadLine(Quoted(q1), Quoted(q2), Quoted(q3));
        std::wcout << q1 << ',' << q2 << ',' << q3 << std::endl;
        std::wstring one, two, three;
        reader.ReadLine(one, two, three);
        std::wcout << one << ',' << two << ',' << three << std::endl;
        int i; float f; double d;
        reader.ReadLine(i, f, d);
        std::wcout << i << ',' << f << ',' << d << std::endl;
        std::wcout << reader.ReadLine<Hexadecimal>()() << std::endl;
        std::wcout << reader.ReadLine<FifamDate>().ToString() << std::endl;
        std::wstring first, second;
        reader.ReadLineWithSeparator('|', first, second);
        std::wcout << first << ',' << second << std::endl;
        intAry = reader.ReadLineArray<int>();
        for (size_t i = 0; i < intAry.size(); i++) {
            if (i != 0)
                std::wcout << ',';
            std::wcout << intAry[i];
        }
        std::wcout << std::endl;
        strAry = reader.ReadLineArray<std::wstring>();
        for (size_t i = 0; i < strAry.size(); i++) {
            if (i != 0)
                std::wcout << ',';
            std::wcout << strAry[i];
        }
        std::wcout << std::endl;
    }
};
