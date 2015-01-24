#ifndef ENSEMBLER_H
#define ENSEMBLER_H

#include "../model.h"
#include "../item_score.h"

#include <vector>
#include <deque>

namespace rsys {

    template <typename T>
    struct mean_pred {
        template <typename ForwardIt>
        T operator() (ForwardIt begin, ForwardIt end) {
            T value = T();
            int count = 0;
            for (ForwardIt it = begin; it != end; ++it) {
                value += *it;
                ++count;
            }
            return value / count;
        }
    };

    template <typename T, typename PRED>
    class ensembler {
    public:
        typedef item_score<T> item_score_t;

        ensembler(const PRED& predict_func = PRED());

        ensembler& add_model(model<T>* m);
        void learn() noexcept;
        T predict(size_t user_id, size_t item_id) noexcept;
        std::deque<item_score_t> recommend(size_t user_id, int k) noexcept;

    private:
        std::vector<model<T>*> _models;
        PRED _pred;
    };

    template <typename T, typename PRED>
    ensembler<T,PRED>::ensembler(const PRED& predict_func)
        : _models(),
          _pred(predict_func) {
    }

    template <typename T, typename PRED>
    ensembler<T,PRED>& ensembler<T,PRED>::add_model(model<T>* m) {
        _models.push_back(m);
        return *this;
    }

    template <typename T, typename PRED>
    void ensembler<T,PRED>::learn() noexcept {
        for (auto& m : _models) {
            m->learn();
        }
    }

    template <typename T, typename PRED>
    T ensembler<T,PRED>::predict(size_t user_id, size_t item_id) noexcept {
        std::vector<T> predictions;
        predictions.reserve(_models.size());

        for (auto& m : _models) {
            predictions.push_back(m->predict(user_id, item_id));
        }

        return _pred(predictions.begin(), predictions.end());
    }

    template <typename T, typename PRED>
    std::deque<typename ensembler<T,PRED>::item_score_t> ensembler<T,PRED>::recommend(size_t user_id, int k) noexcept {

    }


}

#endif // ENSEMBLER_H