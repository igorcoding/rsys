#ifndef RSYS_H
#define RSYS_H

namespace rsys {

    template<typename M>
    class rsys {
    public:

        const M& get_model() const { return _model; }

    private:
        M _model;
    };

} // namespace rsys

#endif // RSYS_H