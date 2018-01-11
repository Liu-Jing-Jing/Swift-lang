//
//  JJObject.h
//  MKJToolKit
//
//  Created by Mark Lewis on 18-1-10.
//  Copyright (c) 2018年 Mark Lewis. All rights reserved.
//
#ifndef __MKJToolKit__JJObject__
#define __MKJToolKit__JJObject__
#include <iostream>

namespace vl
{
    
#if defined VCZH_MSVC
	typedef signed __int8			vint8_t;
	typedef unsigned __int8			vuint8_t;
	typedef signed __int16			vint16_t;
	typedef unsigned __int16		vuint16_t;
	typedef signed __int32			vint32_t;
	typedef unsigned __int32		vuint32_t;
	typedef signed __int64			vint64_t;
	typedef unsigned __int64		vuint64_t;
#elif defined VCZH_GCC
	typedef int8_t					vint8_t;
	typedef uint8_t					vuint8_t;
	typedef int16_t					vint16_t;
	typedef uint16_t				vuint16_t;
	typedef int32_t					vint32_t;
	typedef uint32_t				vuint32_t;
	typedef int64_t					vint64_t;
	typedef uint64_t				vuint64_t;
#endif

#ifdef __MAC_10_0
    typedef int Int;
    typedef int32_t Int32;
    typedef double Double;
    typedef float Float;
    typedef char* CString;
    
    typedef int vint;
    typedef int8_t vint8_t;
    typedef uint8_t vuint8_t;
    typedef int16_t vint16_t;
    typedef uint16_t vuint16_t;
    typedef int32_t vint32_t;
    typedef uint32_t vuint32_t;
    typedef int64_t vint64_t;
    typedef uint64_t vuint64_t;
#endif
    
    
    class NotCopyable
    {
    private:
        NotCopyable(const NotCopyable&); //复制构造函数
        NotCopyable& operator=(const NotCopyable&); //等号运算符重载
    public:
        NotCopyable(); //默认构造函数
    };
    
    class Error
    {
    private:
        const wchar_t*		description; // 描述
    public:
        Error(const wchar_t* _description);
        
        const wchar_t*		Description()const;
    };
    
#if defined VCZH_MSVC || defined VCZH_GCC || defined _DEBUG
#define CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw Error(DESCRIPTION);}while(0)
#elif defined NDEBUG
#define CHECK_ERROR(CONDITION,DESCRIPTION)
#endif
    
#define CHECK_FAIL(DESCRIPTION) do{throw Error(DESCRIPTION);}while(0)
    
#define SCOPE_VARIABLE(TYPE, VARIABLE, VALUE)\
if(bool __scope_variable_flag__=true)\
for(TYPE VARIABLE = VALUE;__scope_variable_flag__;__scope_variable_flag__=false)
    
    /***********************************************************************
     类型计算
     ***********************************************************************/
    
    template<typename T>
    struct RemoveReference
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveReference<T&>
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveReference<T&&>
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveConst
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveConst<const T>
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveVolatile
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveVolatile<volatile T>
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveCVR
    {
        typedef T Type;
    };
    
    template<typename T>
    struct RemoveCVR<T&>
    {
        typedef typename RemoveCVR<T>::Type Type;
    };
    
    template<typename T>
    struct RemoveCVR<T&&>
    {
        typedef typename RemoveCVR<T>::Type Type;
    };
    
    template<typename T>
    struct RemoveCVR<const T>
    {
        typedef typename RemoveCVR<T>::Type Type;
    };
    
    template<typename T>
    struct RemoveCVR<volatile T>
    {
        typedef typename RemoveCVR<T>::Type	Type;
    };
    
    template<typename T>
    typename RemoveReference<T>::Type&& MoveValue(T&& value)
    {
        return (typename RemoveReference<T>::Type&&)value;
    }
    
    template<typename T>
    T&& ForwardValue(typename RemoveReference<T>::Type&& value)
    {
        return (T&&)value;
    }
    
    template<typename T>
    T&& ForwardValue(typename RemoveReference<T>::Type& value)
    {
        return (T&&)value;
    }
    
    template<typename ...TArgs>
    struct TypeTuple
    {
    };
    
    /***********************************************************************
     基础
     ***********************************************************************/
    
    class Object
    {
    public:
        virtual ~Object();
    };
    
    template<typename T>
    class ObjectBox : public Object
    {
    private:
        T					object;
    public:
        ObjectBox(const T& _object)
        :object(_object)
        {
        }
        
        ObjectBox(T&& _object)
        :object(MoveValue(_object))
        {
        }
        
        ObjectBox(const ObjectBox<T>& value)
        :object(value.object)
        {
        }
        
        ObjectBox(ObjectBox<T>&& value)
        :object(MoveValue(value.object))
        {
        }
        
        ObjectBox<T>& operator=(const T& _object)
        {
            object=_object;
            return *this;
        }
        
        ObjectBox<T>& operator=(const ObjectBox<T>& value)
        {
            object=value.object;
            return *this;
        }
        
        ObjectBox<T>& operator=(ObjectBox<T>&& value)
        {
            object=MoveValue(value.object);
            return *this;
        }
        
        const T& Unbox()
        {
            return object;
        }
    };
    
    template<typename T>
    class Nullable
    {
    private:
        T*					object;
    public:
        Nullable()
        :object(0)
        {
        }
        
        Nullable(const T& value)
        :object(new T(value))
        {
        }
        
        Nullable(T&& value)
        :object(new T(MoveValue(value)))
        {
        }
        
        Nullable(const Nullable<T>& nullable)
        :object(nullable.object?new T(*nullable.object):0)
        {
        }
        
        Nullable(Nullable<T>&& nullable)
        :object(nullable.object)
        {
            nullable.object=0;
        }
        
        ~Nullable()
        {
            if(object)
            {
                delete object;
                object=0;
            }
        }
        
        Nullable<T>& operator=(const T& value)
        {
            if(object)
            {
                delete object;
                object=0;
            }
            object=new T(value);
            return *this;
        }
        
        Nullable<T>& operator=(const Nullable<T>& nullable)
        {
            if(this!=&nullable)
            {
                if(object)
                {
                    delete object;
                    object=0;
                }
                if(nullable.object)
                {
                    object=new T(*nullable.object);
                }
            }
            return *this;
        }
        
        Nullable<T>& operator=(Nullable<T>&& nullable)
        {
            if(this!=&nullable)
            {
                if(object)
                {
                    delete object;
                    object=0;
                }
                object=nullable.object;
                nullable.object=0;
            }
            return *this;
        }
        
        static bool Equals(const Nullable<T>& a, const Nullable<T>& b)
        {
            return
            a.object
            ?b.object
            ?*a.object==*b.object
            :false
            :b.object
            ?false
            :true;
        }
        
        static vint Compare(const Nullable<T>& a, const Nullable<T>& b)
        {
            return
            a.object
            ?b.object
            ?(*a.object==*b.object?0:*a.object<*b.object?-1:1)
            :1
            :b.object
            ?-1
            :0;
        }
        
        bool operator==(const Nullable<T>& nullable)const
        {
            return Equals(*this, nullable);
        }
        
        bool operator!=(const Nullable<T>& nullable)const
        {
            return !Equals(*this, nullable);
        }
        
        bool operator<(const Nullable<T>& nullable)const
        {
            return Compare(*this, nullable)<0;
        }
        
        bool operator<=(const Nullable<T>& nullable)const
        {
            return Compare(*this, nullable)<=0;
        }
        
        bool operator>(const Nullable<T>& nullable)const
        {
            return Compare(*this, nullable)>0;
        }
        
        bool operator>=(const Nullable<T>& nullable)const
        {
            return Compare(*this, nullable)>=0;
        }
        
        operator bool()const
        {
            return object!=0;
        }
        
        const T& Value()const
        {
            return *object;
        }
    };
    
    template<typename T, size_t minSize>
    union BinaryRetriver
    {
        T t;
        char binary[sizeof(T)>minSize?sizeof(T):minSize];
    };
    
    /***********************************************************************
     配置
     ***********************************************************************/
    
    template<typename T>
    struct KeyType
    {
    public:
        typedef T Type;
        
        static T GetKeyValue(const T& value)
        {
            return value;
        }
    };
    
    template<typename T>
    struct POD
    {
        static const bool Result=false;
    };
    
    template<>struct POD<bool>{static const bool Result=true;};
    template<>struct POD<vint8_t>{static const bool Result=true;};
    template<>struct POD<vuint8_t>{static const bool Result=true;};
    template<>struct POD<vint16_t>{static const bool Result=true;};
    template<>struct POD<vuint16_t>{static const bool Result=true;};
    template<>struct POD<vint32_t>{static const bool Result=true;};
    template<>struct POD<vuint32_t>{static const bool Result=true;};
    template<>struct POD<vint64_t>{static const bool Result=true;};
    template<>struct POD<vuint64_t>{static const bool Result=true;};
    template<>struct POD<char>{static const bool Result=true;};
    template<>struct POD<wchar_t>{static const bool Result=true;};
    template<typename T>struct POD<T*>{static const bool Result=true;};
    template<typename T>struct POD<T&>{static const bool Result=true;};
    template<typename T, typename C>struct POD<T C::*>{static const bool Result=true;};
    template<typename T, vint _Size>struct POD<T[_Size]>{static const bool Result=POD<T>::Result;};
    template<typename T>struct POD<const T>{static const bool Result=POD<T>::Result;};
    template<typename T>struct POD<volatile T>{static const bool Result=POD<T>::Result;};
    template<typename T>struct POD<const volatile T>{static const bool Result=POD<T>::Result;};
    
    /***********************************************************************
     时间
     ***********************************************************************/
    struct DateTime
    {
        vint				year;
        vint				month;
        vint				dayOfWeek;
        vint				day;
        vint				hour;
        vint				minute;
        vint				second;
        vint				milliseconds;
        
        vuint64_t			totalMilliseconds;
        
        // in gcc, this will be mktime(t) * 1000 + gettimeofday().tv_usec / 1000
        vuint64_t			filetime;
        
        
        static DateTime		LocalTime();
        static DateTime		UtcTime();
        static DateTime		FromDateTime(vint _year, vint _month, vint _day, vint _hour=0, vint _minute=0, vint _second=0, vint _milliseconds=0);
        
        static DateTime		FromFileTime(vuint64_t filetime);
        
        
        DateTime();
        
        DateTime			ToLocalTime();
        DateTime			ToUtcTime();
        DateTime			Forward(vuint64_t milliseconds);
        DateTime			Backward(vuint64_t milliseconds);
        
        bool operator==(const DateTime& value)const { return filetime==value.filetime; }
        bool operator!=(const DateTime& value)const { return filetime!=value.filetime; }
        bool operator<(const DateTime& value)const { return filetime<value.filetime; }
        bool operator<=(const DateTime& value)const { return filetime<=value.filetime; }
        bool operator>(const DateTime& value)const { return filetime>value.filetime; }
        bool operator>=(const DateTime& value)const { return filetime>=value.filetime; }
    };
    
    /***********************************************************************
     接口
     ***********************************************************************/
    
    class Interface : private NotCopyable
    {
    public:
        virtual ~Interface();
    };
    
    /***********************************************************************
     类型萃取
     ***********************************************************************/
    
    struct YesType{};
    struct NoType{};
    
    template<typename T, typename YesOrNo>
    struct AcceptType
    {
    };
    
    template<typename T>
    struct AcceptType<T, YesType>
    {
        typedef T Type;
    };
    
    template<typename YesOrNo>
    struct AcceptValue
    {
        static const bool Result=false;
    };
    
    template<>
    struct AcceptValue<YesType>
    {
        static const bool Result=true;
    };
    
    template<typename TFrom, typename TTo>
    struct RequiresConvertable
    {
        static YesType Test(TTo* value);
        static NoType Test(void* value);
        
        typedef decltype(Test((TFrom*)0)) YesNoType;
    };
    
    class JJObject
    {
    public:
        virtual ~JJObject();
    };
}


#endif /* defined(__MKJToolKit__JJObject__) */
