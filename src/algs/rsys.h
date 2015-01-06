#ifndef RSYS_H
#define RSYS_H

namespace rsys {

template <typename T, template <class> class D, template <class, template <class> class> class M>
class rsys {
public:
    rsys(const M<T, D>& model);
    rsys(M<T, D>&& model);
    const M<T, D>& get_model() const { return _model; }

    void learn() noexcept;
    T predict(size_t user_id, size_t item_id) noexcept;

private:
    M<T, D> _model;
};

//template <typename T, template <class,class> class M>
//using rsys_matrix = rsys<T, math::matrix, M>;

//template <typename T, template <class,class> class M>
//using rsys_sparse = rsys<T, math::sparse, M>;

template <typename T, template <class> class D, template <class, template <class> class> class M>
rsys<T,D,M>::rsys(const M<T, D>& model)
    : _model(model)
{

}

template <typename T, template <class> class D, template <class, template <class> class> class M>
rsys<T,D,M>::rsys(M<T, D>&& model)
{
    _model = std::move(model);
}

template <typename T, template <class> class D, template <class, template <class> class> class M>
void rsys<T,D,M>::learn() noexcept {
    _model.learn();
}

template <typename T, template <class> class D, template <class, template <class> class> class M>
T rsys<T,D,M>::predict(size_t user_id, size_t item_id) noexcept {
    return _model.predict(user_id, item_id);
}

} // namespace rsys

#endif // RSYS_H