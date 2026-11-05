#include <iostream>
#include <cstring>
using namespace std;

const int MAX = 1000;

// Завдання 1. Префікс-функція
void prefixFunction(const char p[], int lps[])
{
    int m = strlen(p), j = 0;
    if (m == 0) return;

    lps[0] = 0;

    for (int i = 1; i < m; i++)
    {
        while (j > 0 && p[i] != p[j])
            j = lps[j - 1];

        if (p[i] == p[j]) j++;
        lps[i] = j;
    }
}

// Завдання 2. Перевірка префікс-функції
bool testPrefix(const char p[], const int expected[])
{
    int lps[MAX];
    prefixFunction(p, lps);

    for (int i = 0; p[i] != '\0'; i++)
    {
        if (lps[i] != expected[i])
        {
            cout << "Помилка: індекс " << i
                << ", символ " << p[i]
                << ", очікувано " << expected[i]
                << ", отримано " << lps[i] << "\n";
            return false;
        }
    }
    return true;
}

// Завдання 3. Пошук КМП
// all = false - перше входження
// all = true  - усі входження
int kmp(const char text[], const char p[], int pos[],
    int& comparisons, int& shifts, bool all = false,
    bool trace = false)
{
    int n = strlen(text), m = strlen(p);
    comparisons = shifts = 0;

    if (m == 0)
    {
        pos[0] = 0;
        return 1;
    }
    if (m > n) return 0;

    int lps[MAX];
    prefixFunction(p, lps);

    int i = 0, j = 0, count = 0;

    while (i < n)
    {
        comparisons++;
        bool match = text[i] == p[j];

        if (trace)
            cout << "i=" << i << " j=" << j
            << " (" << text[i] << "," << p[j] << ") "
            << (match ? "збіг" : "не збіг") << " ";

        if (match)
        {
            i++;
            j++;

            if (trace) cout << "нове j=" << j << "\n";

            if (j == m)
            {
                pos[count++] = i - m;

                if (trace)
                    cout << "Знайдено: " << i - m << "\n";

                if (!all) return count;

                int old = j;
                j = lps[j - 1];
                shifts += old - j;

                if (trace)
                    cout << "LPS: j=" << j << "\n";
            }
        }
        else if (j > 0)
        {
            int old = j;
            j = lps[j - 1];
            shifts += old - j;

            if (trace)
                cout << "LPS: нове j=" << j << "\n";
        }
        else
        {
            i++;
            shifts++;
            if (trace) cout << "нове j=0\n";
        }
    }
    return count;
}

// Прямий пошук для порівняння
int directSearch(const char text[], const char p[],
    int& comparisons, int& shifts)
{
    int n = strlen(text), m = strlen(p);
    comparisons = shifts = 0;

    if (m == 0) return 0;

    for (int i = 0; i <= n - m; i++)
    {
        int j = 0;

        while (j < m)
        {
            comparisons++;
            if (text[i + j] != p[j]) break;
            j++;
        }

        if (j == m) return i;
        shifts++;
    }
    return -1;
}

// Завдання 6. Автоматична перевірка пошуку
bool testSearch(const char text[], const char p[],
    const int expected[], int size)
{
    int pos[MAX], c, s;
    int count = kmp(text, p, pos, c, s, true);

    if (count != size) return false;

    for (int i = 0; i < size; i++)
        if (pos[i] != expected[i]) return false;

    int first = kmp(text, p, pos, c, s);
    int result = first ? pos[0] : -1;
    int correct = size ? expected[0] : -1;

    return result == correct;
}

// Порівняння алгоритмів
void compare(const char text[], const char p[])
{
    int pos[MAX], c1, c2, s1, s2;

    int direct = directSearch(text, p, c1, s1);
    int count = kmp(text, p, pos, c2, s2);
    int first = count ? pos[0] : -1;

    cout << "Прямий: позиція " << direct
        << ", порівнянь " << c1
        << ", зміщень " << s1 << "\n";

    cout << "КМП:    позиція " << first
        << ", порівнянь " << c2
        << ", зміщень " << s2 << "\n";

    cout << "Зекономлено порівнянь: " << c1 - c2 << "\n";
}

int main()
{
    const char text[] = "testtestertesttest";
    const char pattern[] = "test";

    int lps[MAX], pos[MAX], c, s;

    // Завдання 1
    cout << "\nЗАВДАННЯ 1. ПРЕФІКС-ФУНКЦІЯ\n";
    prefixFunction(pattern, lps);

    cout << "Індекси: ";
    for (int i = 0; i < strlen(pattern); i++)
        cout << i << " ";

    cout << "\nСимволи: ";
    for (int i = 0; i < strlen(pattern); i++)
        cout << pattern[i] << " ";

    cout << "\nLPS:     ";
    for (int i = 0; i < strlen(pattern); i++)
        cout << lps[i] << " ";
    cout << "\n";

    // Завдання 2
    cout << "\nЗАВДАННЯ 2. ПЕРЕВІРКА LPS\n";

    const char* samples[6] = {
        "a", "aaaa", "abcd",
        "ababaca", "aabaaab", "abcababcab"
    };

    int expectedLPS[6][10] = {
        {0},
        {0, 1, 2, 3},
        {0, 0, 0, 0},
        {0, 0, 1, 2, 3, 0, 1},
        {0, 1, 0, 1, 2, 2, 3},
        {0, 0, 0, 1, 2, 1, 2, 3, 4, 5}
    };

    for (int i = 0; i < 6; i++)
        cout << "Тест " << i + 1 << ": "
        << (testPrefix(samples[i], expectedLPS[i])
            ? "успішно" : "помилка") << "\n";

    // Завдання 3
    cout << "\nЗАВДАННЯ 3. ПЕРШЕ ВХОДЖЕННЯ\n";

    int count = kmp(text, pattern, pos, c, s);
    cout << "Позиція: " << (count ? pos[0] : -1) << "\n";
    cout << "Порівнянь: " << c << "\n";
    cout << "Зміщень: " << s << "\n";

    // Завдання 4
    cout << "\nЗАВДАННЯ 4. УСІ ВХОДЖЕННЯ\n";

    count = kmp(text, pattern, pos, c, s, true);
    cout << "Позиції: ";
    for (int i = 0; i < count; i++)
        cout << pos[i] << " ";
    cout << "\nКількість: " << count << "\n";

    // Завдання 5
    cout << "\nЗАВДАННЯ 5. ТРАСУВАННЯ\n";
    kmp("abababa", "aba", pos, c, s, true, true);

    // Завдання 6
    cout << "\nЗАВДАННЯ 6. АВТОМАТИЧНІ ТЕСТИ\n";

    const char* texts[10] = {
        "algorithm", "aaaa", "abababa", "abc",
        "mississippi", "abcabcabd",
        "no matches here", "",
        "testtestertesttest", "aaaaa"
    };

    const char* patterns[10] = {
        "rith", "aa", "aba", "abcd",
        "issi", "abcabd", "xyz", "a",
        "test", "aaa"
    };

    int expected[10][4] = {
        {4}, {0, 1, 2}, {0, 2, 4}, {0},
        {1, 4}, {3}, {0}, {0},
        {0, 4, 10, 14}, {0, 1, 2}
    };

    int sizes[10] = { 1, 3, 3, 0, 2, 1, 0, 0, 4, 3 };
    int passed = 0;

    for (int i = 0; i < 10; i++)
    {
        bool ok = testSearch(texts[i], patterns[i],
            expected[i], sizes[i]);

        cout << "Тест " << i + 1 << ": "
            << (ok ? "успішно" : "помилка") << "\n";

        if (ok) passed++;
    }

    cout << "Успішно: " << passed << "/10\n";

    // Додатковий тест порожнього зразка
    int emptyExpected[] = { 0 };
    cout << "Порожній зразок: "
        << (testSearch("abc", "", emptyExpected, 1)
            ? "успішно" : "помилка") << "\n";

    // Завдання 7
    cout << "\nЗАВДАННЯ 7. ВАРІАНТ 7\n";
    cout << "Текст: " << text << "\n";
    cout << "Зразок: " << pattern << "\n";

    count = kmp(text, pattern, pos, c, s, true);
    cout << "Усі позиції: ";
    for (int i = 0; i < count; i++)
        cout << pos[i] << " ";
    cout << "\n";

    cout << "\nПОРІВНЯННЯ АЛГОРИТМІВ\n";
    compare(text, pattern);

    // Високий рівень
    cout << "\nВИСОКИЙ РІВЕНЬ. СКЛАДНІ ДАНІ\n";

    char difficult[32];
    for (int i = 0; i < 30; i++)
        difficult[i] = 'a';
    difficult[30] = 'b';
    difficult[31] = '\0';

    cout << "\nНабір 1. Повторення a\n";
    compare(difficult, "aaaaaaaaab");

    cout << "\nНабір 2. Повторення ab\n";
    compare("ababababababababac", "ababac");

    cout << "\nВласний зразок: abababac\n";
    prefixFunction("abababac", lps);

    cout << "LPS: ";
    for (int i = 0; i < 8; i++)
        cout << lps[i] << " ";
    cout << "\n";

    return 0;
}
