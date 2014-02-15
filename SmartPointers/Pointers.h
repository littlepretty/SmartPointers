//
//  Pointers.h
//  SmartPointers
//
//  Created by 闫 佳琪 on 3/12/13.
//  Copyright (c) 2013 littlepretty. All rights reserved.
//

#ifndef __SmartPointers__Pointers__
#define __SmartPointers__Pointers__

#include <iostream>

/**
 *	Act like auto_ptr<>
 */
template <typename T>
class Pointer
{
    
protected:
    T* mPtr;
    
public:
    Pointer(T* ptr);
    ~Pointer();
    
    T& operator* ();
    const T& operator* () const;
    
    T* operator-> ();
    const T* operator-> () const;
    
    operator void*() const{ return mPtr; }
    
    
private:
    /**
     *	Do not allow copy and assignment
     */
    Pointer(const Pointer<T>& src);
    Pointer<T>& operator=(const Pointer<T>& rhs);
    
};


template <typename T>
Pointer<T>::Pointer(T* ptr)
{
    mPtr=ptr;
}

template <typename T>
Pointer<T>::~Pointer()
{
    delete mPtr;
}

template <typename T>
T& Pointer<T>::operator* ()
{
    return (*mPtr);
}

template <typename T>
T* Pointer<T>::operator-> ()
{
    return mPtr;
}


/**
 *	simple auto_ptr
 */
template <typename T>
class AutoPointer {
    
private:
    T *mPtr;
    
public:
    explicit AutoPointer(T* ptr): mPtr(ptr) {}
    
    ~AutoPointer() {
        delete mPtr;
    }
    
    /**
     *	operation * and ->
     */
    const T& operator*() const {
        return *mPtr;
    }
    
    const T* operator->() const {
        return mPtr;
    }
    
    T& operator*() {
        return *mPtr;
    }
    
    T* operator->() {
        return mPtr;
    }
    
    /**
     *	assignment and copy constructor
     */
    template <typename U>
    AutoPointer<T>& operator=(AutoPointer<U>& rhs) {
        if (&rhs != this) {
            mPtr = rhs.release();
            return *this;
        }
    }
    
    template <typename U>
    AutoPointer(AutoPointer<U>& rhs): mPtr(rhs.release()){
        
    }

    /**
     *	resource management
     */
    T* get() const {
        return mPtr;
    }
    
    T* release() {
        T* old = mPtr;
        mPtr = 0;
        return old;
    }
    
    void reset(T *p = 0) {
        if (p != mPtr) {
            delete mPtr;
            mPtr = p;
        }
    }
    
    
    
};


#include <map>

/**
 *	Act like shared_ptr<>
 */
template <typename T>
class SuperSmartPointer
{
public:
    explicit SuperSmartPointer(T* ptr);
    ~SuperSmartPointer();
    
    /**
     *	Allow copy and assignment
     */
    SuperSmartPointer(const SuperSmartPointer<T>& src);
    SuperSmartPointer<T>& operator=(const SuperSmartPointer<T>& rhs);

    const T& operator*() const;
    const T* operator->() const;
    T& operator*();
    T* operator->();
    
    operator void*() const{return mPtr;}
    
protected:
    
    T* mPtr;
    static std::map<T*, int> sRefenceCountMap;
    
    void finalizePointer();
    void initPointer(T* ptr);
    
};

template <typename T>
std::map<T*, int>SuperSmartPointer<T>::sRefenceCountMap;

template <typename T>
SuperSmartPointer<T>::SuperSmartPointer(T* ptr)
{
    initPointer(ptr);
}

template <typename T>
SuperSmartPointer<T>& SuperSmartPointer<T>::operator=(const SuperSmartPointer<T>& rhs)
{
    if(this==rhs){
        return (*this);
    }
    finalizePointer();
    initPointer(rhs.mPtr);
    
    return (*this);
}

template <typename T>
SuperSmartPointer<T>::~SuperSmartPointer<T>()
{
    finalizePointer();
}

template <typename T>
T& SuperSmartPointer<T>::operator*()
{
    return (*mPtr);
}

template <typename T>
T* SuperSmartPointer<T>::operator->()
{
    return mPtr;
}

template <typename T>
const T& SuperSmartPointer<T>::operator*() const
{
    return (*mPtr);
}

template <typename T>
const T* SuperSmartPointer<T>::operator->() const
{
    return mPtr;
}

template <typename T>
void SuperSmartPointer<T>::initPointer(T *ptr)
{
    mPtr=ptr;
    /**
     *	update reference count table
     */
    if(sRefenceCountMap.find(mPtr)==sRefenceCountMap.end())
    {
        sRefenceCountMap[mPtr]=1;
    }else{
        sRefenceCountMap[mPtr]++;
    }
}

template <typename T>
void SuperSmartPointer<T>::finalizePointer()
{
    if(sRefenceCountMap.find(mPtr)==sRefenceCountMap.end())
    {
        std::cerr<<"ERROR: Missing entry in map!"<<std::endl;
        return;
    }
    sRefenceCountMap[mPtr]--;
    if(sRefenceCountMap[mPtr] == 0)
    {
        sRefenceCountMap.erase(mPtr);
        delete mPtr;
    }
}


class Nothing
{
public:
    Nothing(){sNumAllocations++;}
    ~Nothing(){sNumDeletions++;}
    
    static int sNumAllocations;
    static int sNumDeletions;
    
};

int Nothing::sNumAllocations=0;
int Nothing::sNumDeletions=0;



#endif /* defined(__SmartPointers__Pointers__) */







