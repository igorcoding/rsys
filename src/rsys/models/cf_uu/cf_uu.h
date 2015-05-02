#ifndef RSYS_CF_UU_H
#define RSYS_CF_UU_H

#include "../model.h"
#include "cf_uu_config.h"
#include "rsys/item_score.h"

namespace rsys {

    template <typename T>
    class cf_uu : model<T> {
    public:
        typedef config<cf_uu<T>> config_t;
        typedef item_score<T> item_score_t;

        cf_uu(const config_t& conf);

        template <typename FwdIt> void train(FwdIt begin, FwdIt end);

    private:

    };


    template <typename T>
    cf_uu<T>::cf_uu() {

    }

    template <typename T>
    template <typename FwdIt>
    void cf_uu<T>::train(FwdIt begin, FwdIt end) {

    }


}  // namespace rsys

#endif //RSYS_CF_UU_H
