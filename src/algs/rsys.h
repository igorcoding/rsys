#ifndef RSYS_H
#define RSYS_H

namespace rsys {

template <typename M>
class rsys {
public:
    rsys(const M& model);
    rsys(M&& model);
    const M& get_model() const { return _model; }

private:
    M _model;
};

template <typename M>
rsys<M>::rsys(const M& model)
    : _model(model)
{

}

template <typename M>
rsys<M>::rsys(M&& model)
{

}

} // namespace rsys

#endif // RSYS_H