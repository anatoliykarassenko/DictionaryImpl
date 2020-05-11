/*Условие задачи
Необходимо реализовать класс контейнера, реализующий интерфейс Dictionary и способный выбросить
исключение, реализующее интерфейс NotFoundException.
При разработке допускается использовать STL. Кроме этого, внутренняя реализация ничем не ограничена.
*/
#include <exception>
#include <unordered_map>
#include <iterator>
#include <shared_mutex>

template<class TKey, class TValue>
class Dictionary
{
public:
    virtual ~Dictionary() = default;

    virtual const TValue& Get(const TKey& key) const = 0;
    virtual void Set(const TKey& key, const TValue& value) = 0;
    virtual bool IsSet(const TKey& key) const = 0;
};

template<class TKey>
class NotFoundException : public std::exception
{
public:
    virtual const TKey& GetKey() const noexcept = 0;
};

//                         Реализация исключения

template<class TKey>
class NotFoundExceptionImpl final : public NotFoundException<TKey>
{
    public:
        NotFoundExceptionImpl(TKey key): _key(std::move(key)){}

        const TKey& GetKey() const noexcept override
        {
            return _key;
        }
        const char* what() const noexcept override
        {
            return "No such key in that dictionary.";
        }
    private:
        TKey _key;
};

//                          Реализация словаря

template<class TKey, class TValue>
class DictionaryImpl : public Dictionary<TKey, TValue>
{
    public:
        DictionaryImpl() = default;
        DictionaryImpl(const DictionaryImpl&) = delete;
        DictionaryImpl(DictionaryImpl&&) = delete;
        DictionaryImpl& operator = (const DictionaryImpl&) = delete;
		DictionaryImpl& operator = (DictionaryImpl&&) = delete;
		~DictionaryImpl() override = default;

        const TValue& Get(const TKey& key) const override
        {
            std::shared_lock<std::shared_mutex> shared_lck(_mut);
            const auto search = _data.find(key);
            if (search == std::end(_data))
            {
                throw NotFoundExceptionImpl<TKey>(key);
            }
            return search->second;
        }
        void Set(const TKey& key, const TValue& value) override
        {
            std::unique_lock<std::shared_mutex> unique_lck(_mut);
            const auto search = _data.find(key);
            if (search == std::end(_data))
            {
                _data.emplace(key, value);
            }
            else
            {
                search->second = value;
            }
        }
        bool IsSet(const TKey& key) const override
        {
            std::shared_lock<std::shared_mutex> shared_lck(_mut);
            const auto search = _data.find(key);
            return search != std::end(_data);
        }
    private:
        std::unordered_map<TKey, TValue> _data;
        mutable std::shared_mutex _mut;
};
