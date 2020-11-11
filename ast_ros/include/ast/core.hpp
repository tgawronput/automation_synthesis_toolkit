#pragma once

#ifdef __cplusplus


#include <map>
#include <type_traits>

#include <delegate/delegate.hpp>



namespace ast
{

template <typename TSignature>
using Fun = srutil::delegate<Signature>;

template<typename T>
struct IsDerived
{
private:
  typedef std::true_type yes;
  typedef std::false_type no;

  template<typename U> static auto test(int) -> decltype(std::declval<U>().isDerived() == true, yes());

  template<typename> static no test(...);

public:

  static constexpr bool value = std::is_same<decltype(test<T>(0)),yes>::value;
};

template<typename T>
struct Base : public T
{
  typedef T BaseT;
  bool isDerived() {return true;}
};

template <class T, class... TArgs> decltype(void(T{std::declval<TArgs>()...}), std::true_type{}) test_is_braces_constructible(int);
template <class, class...> std::false_type test_is_braces_constructible(...);
template <class T, class... TArgs> using is_braces_constructible = decltype(test_is_braces_constructible<T, TArgs...>(0));

struct any_type
{
  template<class T>
  constexpr operator T(); // non explicit
};

//TODO: we could make a constexpr function out of this probably and add namespace handling
template <typename _Get_TypeName>
const char* getTypeName()
{
  static std::string name;

  if (name.empty())
  {
    const char *beginStr = "_Get_TypeName =";
    const size_t beginStrLen = 15;
    size_t begin,length;
    name = __PRETTY_FUNCTION__;
    begin = name.find(beginStr) + beginStrLen + 1;
    length = name.find("]",begin) - begin;
    name = name.substr(begin,length);
    name = name.substr(0, name.find(";", 0));
  }

  return name.c_str();
}

struct DefaultConfig
{
  template <typename T>
  T* create()
  {
    return new T();
  }
};

template <typename Config = DefaultConfig>
class Context;

template <typename T>
class In
{
  T* data = nullptr;
  const char* name_;
public:
  template <typename Config>
  friend class Context;
  In(const char* name = "") : name_(name)
  {}

  const T& operator()()
  {
    return *data;
  }
};


template <typename T>
class Out
{
  T* data = nullptr;
  const char* name_;
public:
  template <typename Config>
  friend class Context;
  Out(const char* name = "") : name_(name)
  {}

  T& operator()()
  {
    return *data;
  }
};

template <typename T>
class Dep
{
  T* data = nullptr;
public:
  template <typename Config>
  friend class Context;
  T& operator()()
  {
    return *data;
  }
};

template <typename T>
class Setting
{
  T* data = nullptr;
  const char* name_;
public:
  template <typename Config>
  friend class Context;
  Setting(const char* name) : name_(name)
  {}
  T& operator()()
  {
    return *data;
  }
};



template <typename Config = DefaultConfig>
class Context
{
  Config config;

  std::map<std::pair<const char*, const char*>, void*> dependencies;
  std::map<std::pair<const char*, const char*>, void*> signals;
  std::map<std::pair<const char*, const char*>, void*> settings;


  template <typename T>
  T* buildFromScratch()
  {
    auto name = getTypeName<T>();
    auto key = std::make_pair(name, "");
    auto it = dependencies.find(key);
    if(it == dependencies.end())
    {
      auto obj = config.template create<T>();
      traverse(*obj);
      //we bypass the shared pointer since modules do not own entities we inject them with
      dependencies[key] = (void*)obj;

      return obj;
    }
    else
    {
      return (T*) it->second;
    }
  }


public:
  static Context<Config>& global()
  {
    static Context<Config> ctx;
    return ctx;
  }

  //just a convenience function
  Context<Config> makeNested()
  {
    return *this;
  }


  template <typename T>
  T& build()
  {
    static T* instance = buildFromScratch<T>();
    return*instance;
  }


  //need to process bigger objects? add a case, no other way before C++2x comes
  template<class T>
  void traverse(T& object) noexcept
  {
    using type = std::decay_t<T>;

    if constexpr(IsDerived<T>::value)
    {

      if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
      {
        auto& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = object;
            resolve(p1);
            resolve(p2);
            resolve(p3);
            resolve(p4);
            resolve(p5);
            resolve(p6);
            resolve(p7);
            resolve(p8);
            resolve(p9);
            resolve(p10);
      }
            else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
        {
          auto& [p1, p2, p3, p4, p5, p6, p7, p8, p9] = object;
              resolve(p1);
              resolve(p2);
              resolve(p3);
              resolve(p4);
              resolve(p5);
              resolve(p6);
              resolve(p7);
              resolve(p8);
              resolve(p9);
        }
              else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
          {
            auto& [p1, p2, p3, p4, p5, p6, p7, p8] = object;
                resolve(p1);
                resolve(p2);
                resolve(p3);
                resolve(p4);
                resolve(p5);
                resolve(p6);
                resolve(p7);
                resolve(p8);
          }
                else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
            {
              auto& [p1, p2, p3, p4, p5, p6, p7] = object;
                  resolve(p1);
                  resolve(p2);
                  resolve(p3);
                  resolve(p4);
                  resolve(p5);
                  resolve(p6);
                  resolve(p7);
            }
                  else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type, any_type, any_type, any_type>{})
              {
                auto& [p1, p2, p3, p4, p5, p6] = object;
                    resolve(p1);
                    resolve(p2);
                    resolve(p3);
                    resolve(p4);
                    resolve(p5);
                    resolve(p6);
              }
                    else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type, any_type, any_type>{})
                {
                  auto& [p1, p2, p3, p4, p5] = object;
                      resolve(p1);
                      resolve(p2);
                      resolve(p3);
                      resolve(p4);
                      resolve(p5);
                }
                      else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type, any_type>{})
                  {
                    auto& [p1, p2, p3, p4] = object;
                        resolve(p1);
                        resolve(p2);
                        resolve(p3);
                        resolve(p4);
                  }
                        else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type, any_type>{})
                    {
                      auto& [p1, p2, p3] = object;
                          resolve(p1);
                          resolve(p2);
                          resolve(p3);
                    }
                          else if constexpr(is_braces_constructible<typename type::BaseT, any_type, any_type>{})
                      {
                        auto& [p1, p2] = object;
                            resolve(p1);
                            resolve(p2);
                      }
                            else if constexpr(is_braces_constructible<typename type::BaseT, any_type>{})
                        {
                          auto& [p1] = object;
                          resolve(p1);
                        }
                      }
                      else
                      {
                        if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
                        {
                          auto& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12] = object;
                              resolve(p1);
                              resolve(p2);
                              resolve(p3);
                              resolve(p4);
                              resolve(p5);
                              resolve(p6);
                              resolve(p7);
                              resolve(p8);
                              resolve(p9);
                              resolve(p10);
                              resolve(p11);
                              resolve(p12);
                        }
                              else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
                          {
                            auto& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] = object;
                                resolve(p1);
                                resolve(p2);
                                resolve(p3);
                                resolve(p4);
                                resolve(p5);
                                resolve(p6);
                                resolve(p7);
                                resolve(p8);
                                resolve(p9);
                                resolve(p10);
                                resolve(p11);
                          }
                                else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
                            {
                              auto& [p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = object;
                                  resolve(p1);
                                  resolve(p2);
                                  resolve(p3);
                                  resolve(p4);
                                  resolve(p5);
                                  resolve(p6);
                                  resolve(p7);
                                  resolve(p8);
                                  resolve(p9);
                                  resolve(p10);
                            }
                                  else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
                              {
                                auto& [p1, p2, p3, p4, p5, p6, p7, p8, p9] = object;
                                    resolve(p1);
                                    resolve(p2);
                                    resolve(p3);
                                    resolve(p4);
                                    resolve(p5);
                                    resolve(p6);
                                    resolve(p7);
                                    resolve(p8);
                                    resolve(p9);
                              }
                                    else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
                                {
                                  auto& [p1, p2, p3, p4, p5, p6, p7, p8] = object;
                                      resolve(p1);
                                      resolve(p2);
                                      resolve(p3);
                                      resolve(p4);
                                      resolve(p5);
                                      resolve(p6);
                                      resolve(p7);
                                      resolve(p8);
                                }
                                      else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type, any_type, any_type>{})
                                  {
                                    auto& [p1, p2, p3, p4, p5, p6, p7] = object;
                                        resolve(p1);
                                        resolve(p2);
                                        resolve(p3);
                                        resolve(p4);
                                        resolve(p5);
                                        resolve(p6);
                                        resolve(p7);
                                  }
                                        else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type, any_type>{})
                                    {
                                      auto& [p1, p2, p3, p4, p5, p6] = object;
                                          resolve(p1);
                                          resolve(p2);
                                          resolve(p3);
                                          resolve(p4);
                                          resolve(p5);
                                          resolve(p6);
                                    }
                                          else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type, any_type>{})
                                      {
                                        auto& [p1, p2, p3, p4, p5] = object;
                                            resolve(p1);
                                            resolve(p2);
                                            resolve(p3);
                                            resolve(p4);
                                            resolve(p5);
                                      }
                                            else if constexpr(is_braces_constructible<type, any_type, any_type, any_type, any_type>{})
                                        {
                                          auto& [p1, p2, p3, p4] = object;
                                              resolve(p1);
                                              resolve(p2);
                                              resolve(p3);
                                              resolve(p4);
                                        }
                                              else if constexpr(is_braces_constructible<type, any_type, any_type, any_type>{})
                                          {
                                            auto& [p1, p2, p3] = object;
                                                resolve(p1);
                                                resolve(p2);
                                                resolve(p3);
                                          }
                                                else if constexpr(is_braces_constructible<type, any_type, any_type>{})
                                            {
                                              auto& [p1, p2] = object;
                                                  resolve(p1);
                                                  resolve(p2);
                                            }
                                                  else if constexpr(is_braces_constructible<type, any_type>{})
                                              {
                                                auto& [p1] = object;
                                                resolve(p1);
                                              }
                                            }
                                          }

                                          template <typename T>
                                          void resolve(T& obj)
                                          {
                                          }

                                          template <typename T>
                                          void resolve(Dep<T>& dep)
                                          {
                                            dep.data = build<T>();
                                          }

                                          template <typename T>
                                          void resolve(In<T>& in)
                                          {
                                            auto key = std::make_pair(getTypeName<T>(), in.name_);
                                            auto it = signals.find(key);
                                            if(it == signals.end())
                                            {
                                              auto obj = config.template create<T>();
                                              signals[key] = (void*)obj;
                                              in.data = obj;
                                            }
                                            else
                                            {
                                              in.data = (T*) it->second;
                                            }
                                          }

                                          template <typename T>
                                          void resolve(Out<T>& in)
                                          {
                                            auto key = std::make_pair(getTypeName<T>(), in.name_);
                                            auto it = signals.find(key);
                                            if(it == signals.end())
                                            {
                                              auto obj = config.template create<T>();
                                              signals[key] = (void*)obj;
                                              in.data = obj;
                                            }
                                            else
                                            {
                                              in.data = (T*) it->second;
                                            }
                                          }

                                          template <typename T>
                                          void resolve(Setting<T>& in)
                                          {
                                            auto key = std::make_pair(getTypeName<T>(), in.name_);
                                            auto it = settings.find(key);
                                            if(it == settings.end())
                                            {
                                              auto obj = config.template create<T>();
                                              settings[key] = (void*)obj;
                                              in.data = obj;
                                            }
                                            else
                                            {
                                              in.data = (T*) it->second;
                                            }
                                          }
                                        };

                                        template <typename T, typename... Rest>
                                        class ModuleSequence : public ModuleSequence<Rest...>
                                        {
                                          T module_;
                                        public:
                                          ModuleSequence(T module, Rest... r) : module_(module), ModuleSequence<Rest...>(r...)
                                          {}

                                          void update()
                                          {
                                            module_->update();
                                            ModuleSequence<Rest...>::update();
                                          }
                                        };

                                        template <typename T>
                                        class ModuleSequence<T>
                                        {
                                          T module_;
                                        public:
                                          ModuleSequence(T module) : module_(module)
                                          {}

                                          void update()
                                          {
                                            module_->update();
                                          }
                                        };


                                        template <typename... T>
                                        ModuleSequence<T...> composeSequence(T... args)
                                        {
                                          return ModuleSequence<T...>(args...);
                                        }

                                      }


#endif
