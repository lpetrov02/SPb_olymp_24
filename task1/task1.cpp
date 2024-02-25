#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>


struct Cell {
    Cell(size_t x, size_t max_n) {
        value = x;
        if (x == 0) {
            for (size_t variant = 1; variant <= max_n; ++variant) {
                variants.insert(variant);
            }
        }
    }

    void Set(size_t k) {
        if (k > 0) {
            variants.clear();
        }
        value = k;
    }

    bool Empty() const {
        return value == 0;
    }

    bool IsCandidate(size_t k) const {
        return Empty() && std::find(variants.begin(), variants.end(), k) != variants.end();
    }

    bool Determined() const {
        return Empty() && variants.size() == 1;
    }

    bool Valid() const {
        return !Empty() || !variants.empty();
    }

    size_t value;
    std::unordered_set<int> variants;
};


enum SetReturn {
    NO_ACTIONS_NEEDED = 0,
    NEEDS_SIMPLIFICATION = 1,
    INVALID_STATE = 2
} ;


struct Field {
    Field(size_t n) : n2(n * n), n(n) {
        cells = std::vector<std::vector<Cell>>(n2, std::vector<Cell>(n2, Cell(0, n2)));
        to_go = n2 * n2;
        sum_vars = to_go * n2;
    }

    Field(const Field& other) {
        cells = other.cells;
        to_go = other.to_go;
        sum_vars = other.sum_vars;
        n2 = other.n2;
        n = other.n;
    }

    void SetStats() {
        sum_vars = 0, to_go = 0;
        for (size_t j = 0; j < n2; ++j) {
            for (size_t i = 0; i < n2; ++i) {
                sum_vars += cells[j][i].variants.size();
                to_go += static_cast<size_t>(cells[j][i].Empty());
            }
        }
    }

    bool IsValid() const {
        for (size_t j = 0; j < n2; ++j) {
            for (size_t i = 0; i < n2; ++i) {
                if (!cells[j][i].Valid()) {
                    return false;
                }
            }
        }
        return true;
    }
    
    SetReturn Set(size_t j, size_t i, size_t k) {
        SetReturn ret = SetReturn::NO_ACTIONS_NEEDED;
        if (!cells[j][i].Empty()) {
            return ret;
        }

        if (k > 0) {
            --to_go;
            sum_vars -= cells[j][i].variants.size();
        }
        cells[j][i].Set(k);

        for (size_t l = 0; l < n2; ++l) {
            sum_vars -= (static_cast<int>(cells[j][l].IsCandidate(k))
                        + static_cast<int>(cells[l][i].IsCandidate(k)));
            cells[j][l].variants.erase(k);
            cells[l][i].variants.erase(k);
            
            if (cells[j][l].Determined() || cells[l][i].Determined()) {
                ret = SetReturn::NEEDS_SIMPLIFICATION;
            }
            if (!cells[j][l].Valid() || !cells[l][i].Valid()) {
                return SetReturn::INVALID_STATE;
            }
        }

        size_t y = j / n * n, x = i / n * n;
        for (size_t j = y; j < y + n; ++j) {
            for (size_t i = x; i < x + n; ++i) {
                sum_vars -= static_cast<int>(cells[j][i].IsCandidate(k));
                cells[j][i].variants.erase(k);
                if (cells[j][i].Determined()) {
                    ret = SetReturn::NEEDS_SIMPLIFICATION;
                }
                if (!cells[j][i].Valid()) {
                    return SetReturn::INVALID_STATE;
                }
            }
        }
        return ret;
    }

    void Simplify() {
        bool improves = true;
        while (improves) {
            improves = false;
            for (size_t j = 0; j < n2; ++j) {
                for (size_t i = 0; i < n2; ++i) {
                    if (cells[j][i].Determined()) {
                        improves = true;
                        Set(j, i, *(cells[j][i].variants.begin()));
                    }
                }
            }
        }
        SetStats();
    }

    size_t n2, n;
    size_t to_go, sum_vars;
    std::vector<std::vector<Cell>> cells;
};


bool operator<(const Field& first, const Field& second) {
    if (first.to_go == second.to_go) {
        return first.sum_vars > second.sum_vars;
    }
    return first.to_go > second.to_go;
}


size_t Sqrt(size_t n) {
    for (size_t k = 1; k * k <= n; ++k) {
        if (k * k == n) {
            return k;
        }
    }
    return -1;
}


void PrintState(const Field& field) {
    for (size_t j = 0; j < field.n2; ++j) {
        for (size_t i = 0; i < field.n2; ++i) {
            if (i > 0) {
                std::cout << " ";
            }
            std::cout << field.cells[j][i].value;
        }
        std::cout << std::endl;
    }
}


int main() {
    size_t n2, n, k;
    std::cin >> n2;
    n = Sqrt(n2);

    if (n == -1) {
        std::cout << "Invalid field" << std::endl;
        return 0;
    }

    Field f(n);
    for (size_t j = 0; j < n2; ++j) {
        for (size_t i = 0; i < n2; ++i) {
            std::cin >> k;
            if (k != 0) {
                f.Set(j, i, k);
            }
        }
    }
    f.Simplify();

    std::vector<Field> open = { f };

    while (!open.empty()) {
        std::pop_heap(open.begin(), open.end());
        auto v = open.back();
        open.pop_back();

        if (v.to_go == 0) {
            PrintState(v);
            return 0;
        }

        size_t best_i = 0, best_j = 0, min_vars = n2 + 1;
        for (size_t j = 0; j < n2; ++j) {
            for (size_t i = 0; i < n2; ++i) {
                if (v.cells[j][i].Empty() && v.cells[j][i].variants.size() < min_vars) {
                    min_vars = v.cells[j][i].variants.size(), best_i = i, best_j = j;
                }
            }
        }

        for (auto k : v.cells[best_j][best_i].variants) {
            Field new_f = v;
            SetReturn ret = new_f.Set(best_j, best_i, k);
            if (ret == SetReturn::NEEDS_SIMPLIFICATION) {
                new_f.Simplify();
            }
            if (ret != SetReturn::INVALID_STATE && new_f.IsValid()) {
                open.push_back(new_f);
                std::push_heap(open.begin(), open.end());
            }
        }
    }
    std::cout << "Sorry, I have failed your task" << std::endl;
    return 0;
}
