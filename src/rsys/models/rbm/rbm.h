#ifndef RBM_H
#define RBM_H

#include "rsys/models/model.h"
#include "rsys/item_score.h"
#include "rbm_config.h"

namespace rsys {

    template<typename T>
    class rbm : public model<T> {
    public:
        typedef config<rbm<T>> config_t;
        typedef item_score<T> item_score_t;

        rbm(const config_t& conf);
        rbm(config_t&& conf);

        template<typename FwdIt> void train(FwdIt begin, FwdIt end);
        virtual T predict(size_t user_id, size_t item_id);
        virtual std::vector<item_score_t> recommend(size_t user_id, int k);

    private:


    private:
        config_t _conf;
    };


    template<typename T>
    rbm<T>::rbm(const config_t& conf)
        : _conf(conf) {

    }

    template<typename T>
    rbm<T>::rbm(config_t&& conf)
        : _conf(conf) {

    }

    template<typename T>
    template<typename FwdIt>
    void rbm<T>::train(FwdIt begin, FwdIt end) {

    }

    template<typename T>
    T rbm<T>::predict(size_t user_id, size_t item_id) {
        return 0;
    }

    template<typename T>
    std::vector<typename rbm<T>::item_score_t> rbm<T>::recommend(size_t user_id, int k) {
        return std::vector<item_score_t>();
    }
} // namespace rsys

#endif // RBM_H