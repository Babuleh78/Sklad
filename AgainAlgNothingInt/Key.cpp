nt gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Функция для нахождения НОК
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

// Функция для подсчета числа пар
int count_pairs(int g, int l) {
    if (l % g != 0) {
        return 0; // НОК не кратен НОД
    }

    int k = l / g; // Приведем к допустимому значению
    vector<int> divisors;

    // Находим делители k
    for (int i = 1; i * i <= k; ++i) {
        if (k % i == 0) {
            divisors.push_back(i);
            if (i != k / i) {
                divisors.push_back(k / i);
            }
        }
    }

    // Подсчитываем пары (x, y) такие, что НОД(x, y) = 1 и НОК(x, y) = k
    int count = 0;
    for (size_t i = 0; i < divisors.size(); ++i) {
        for (size_t j = i; j < divisors.size(); ++j) {
            int x = divisors[i];
            int y = divisors[j];
            if (gcd(x, y) == 1 && lcm(x, y) == k) {
                if (x == y) {
                    count += 1; // (x, x) - одна пара
                } else {
                    count += 2; // (x, y) и (y, x) - две пары
                }
            }
        }
    }

    return count;
}

int main() {
    int g, l;
    cout << "Введите НОД: ";
    cin >> g;
    cout << "Введите НОК: ";
    cin >> l;

    int result = count_pairs(g, l);
    cout << "Количество подходящих пар: " << result << endl;

    return 0;
}
