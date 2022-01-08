#ifndef IDLECALLBACKHANDLER_H_INCLUDED
#define IDLECALLBACKHANDLER_H_INCLUDED

// Asynchronous method call events: these event are processed by wxEvtHandler
// itself and result in a call to its Execute() method which simply calls the
// specified method. The difference with a simple method call is that this is
// done asynchronously, i.e. at some later time, instead of immediately when
// the event object is constructed.

#include <deque>
#include "wx/event.h"
#include "wx/window.h"
#include "manager.h"

#ifdef HAVE_OVERRIDE
    #define wxOVERRIDE override
#else /*  !HAVE_OVERRIDE */
    #define wxOVERRIDE
#endif /*  HAVE_OVERRIDE */

// This is a base class used to process all method calls.
// ----------------------------------------------------------------------------
class AsyncMethodCallEvent : public wxEvent
// ----------------------------------------------------------------------------
{
public:
    AsyncMethodCallEvent(wxObject* object)
        : wxEvent(wxID_ANY, wxEVT_ASYNC_METHOD_CALL)
    {
        SetEventObject(object);
    }

    AsyncMethodCallEvent(const AsyncMethodCallEvent& other)
        : wxEvent(other)
    {
    }

    virtual void Execute() = 0;
};
// ----------------------------------------------------------------------------
// This is a version for calling methods without parameters.
// ----------------------------------------------------------------------------
template <typename T>
class AsyncMethodCallEvent0 : public AsyncMethodCallEvent
{
public:
    typedef T ObjectType;
    typedef void (ObjectType::*MethodType)();

    AsyncMethodCallEvent0(ObjectType* object,
                            MethodType method)
        : AsyncMethodCallEvent(object),
          m_object(object),
          m_method(method)
    {
    }

    AsyncMethodCallEvent0(const AsyncMethodCallEvent0& other)
        : AsyncMethodCallEvent(other),
          m_object(other.m_object),
          m_method(other.m_method)
    {
    }

    virtual AsyncMethodCallEvent* Clone() const wxOVERRIDE
    {
        return new AsyncMethodCallEvent0(*this);
    }

    virtual void Execute() wxOVERRIDE
    {
        (m_object->*m_method)();
    }

private:
    ObjectType* const m_object;
    const MethodType m_method;
};
// ----------------------------------------------------------------------------
// This is a version for calling methods with a single parameter.
// ----------------------------------------------------------------------------
template <typename T, typename T1>
class AsyncMethodCallEvent1 : public AsyncMethodCallEvent
{
public:
    typedef T ObjectType;
    typedef void (ObjectType::*MethodType)(T1 x1);
    typedef typename wxRemoveRef<T1>::type ParamType1;

    AsyncMethodCallEvent1(ObjectType* object,
                            MethodType method,
                            const ParamType1& x1)
        : AsyncMethodCallEvent(object),
          m_object(object),
          m_method(method),
          m_param1(x1)
    {
    }

    AsyncMethodCallEvent1(const AsyncMethodCallEvent1& other)
        : AsyncMethodCallEvent(other),
          m_object(other.m_object),
          m_method(other.m_method),
          m_param1(other.m_param1)
    {
    }

    virtual AsyncMethodCallEvent* Clone() const wxOVERRIDE
    {
        return new AsyncMethodCallEvent1(*this);
    }

    virtual void Execute() wxOVERRIDE
    {
        (m_object->*m_method)(m_param1);
    }

private:
    ObjectType* const m_object;
    const MethodType m_method;
    /*const*/ ParamType1 m_param1;
};

// ----------------------------------------------------------------------------
// This is a version for calling methods with two parameters.
// ----------------------------------------------------------------------------
template <typename T, typename T1, typename T2>
class AsyncMethodCallEvent2 : public AsyncMethodCallEvent
{
public:
    typedef T ObjectType;
    typedef void (ObjectType::*MethodType)(T1 x1, T2 x2);
    typedef typename wxRemoveRef<T1>::type ParamType1;
    typedef typename wxRemoveRef<T2>::type ParamType2;

    AsyncMethodCallEvent2(ObjectType* object,
                            MethodType method,
                            const ParamType1& x1,
                            const ParamType2& x2)
        : AsyncMethodCallEvent(object),
          m_object(object),
          m_method(method),
          m_param1(x1),
          m_param2(x2)
    {
    }

    AsyncMethodCallEvent2(const AsyncMethodCallEvent2& other)
        : AsyncMethodCallEvent(other),
          m_object(other.m_object),
          m_method(other.m_method),
          m_param1(other.m_param1),
          m_param2(other.m_param2)
    {
    }

    virtual AsyncMethodCallEvent* Clone() const wxOVERRIDE
    {
        return new AsyncMethodCallEvent2(*this);
    }

    virtual void Execute() wxOVERRIDE
    {
        (m_object->*m_method)(m_param1, m_param2);
    }

private:
    ObjectType* const m_object;
    const MethodType m_method;
    const ParamType1 m_param1;
    const ParamType2 m_param2;
};

// ----------------------------------------------------------------------------
// This is a version for calling any functors
// ----------------------------------------------------------------------------
template <typename T>
class AsyncMethodCallEventFunctor : public AsyncMethodCallEvent
{
public:
    typedef T FunctorType;

    AsyncMethodCallEventFunctor(wxObject *object, const FunctorType& fn)
        : AsyncMethodCallEvent(object),
          m_fn(fn)
    {
    }

    AsyncMethodCallEventFunctor(const AsyncMethodCallEventFunctor& other)
        : AsyncMethodCallEvent(other),
          m_fn(other.m_fn)
    {
    }

    virtual AsyncMethodCallEvent* Clone() const wxOVERRIDE
    {
        return new AsyncMethodCallEventFunctor(*this);
    }

    virtual void Execute() wxOVERRIDE
    {
        m_fn();
    }

private:
    FunctorType m_fn;
};//end class AsyncMethodCallEventFunctor


// ----------------------------------------------------------------------------
class IdleCallbackHandler: public wxEvtHandler
// ----------------------------------------------------------------------------
{
  private:
    std::deque<AsyncMethodCallEvent*> m_AsyncMethodCallQueue;

    // ----------------------------------------------------------------------------
    void OnIdle(wxIdleEvent& event)
    // ----------------------------------------------------------------------------
    {
        event.Skip();

        //while (GetIdleCallbackQueue()->size())
        if (GetIdleCallbackQueue()->size())
        {
            AsyncMethodCallEvent* pAsyncCall = GetIdleCallbackQueue()->front();
            AsyncMethodCallEvent* pCall =  (AsyncMethodCallEvent*)pAsyncCall->Clone();
            delete(pAsyncCall);
            GetIdleCallbackQueue()->pop_front();

            pCall->Execute();
            delete(pCall);
        }
    }

  public:

    // Verify that an event handler is still in the chain of event handlers
    wxEvtHandler* FindEventHandler(wxEvtHandler* pEvtHdlr)
    {
        wxEvtHandler* pFoundEvtHdlr =  Manager::Get()->GetAppWindow()->GetEventHandler();

        while (pFoundEvtHdlr != nullptr)
        {
            if (pFoundEvtHdlr == pEvtHdlr)
                return pFoundEvtHdlr;
            pFoundEvtHdlr = pFoundEvtHdlr->GetNextHandler();
        }
        return nullptr;
    }

    // ----------------------------------------------------------------------------
    IdleCallbackHandler()
    // ----------------------------------------------------------------------------
    {
        //ctor
        IdleCallbackHandler* pThis = this;
        Manager::Get()->GetAppWindow()->PushEventHandler(pThis);
        Bind(wxEVT_IDLE, &IdleCallbackHandler::OnIdle, this);
    }
    // ----------------------------------------------------------------------------
    ~IdleCallbackHandler()
    // ----------------------------------------------------------------------------
    {
        //dtor
        Unbind(wxEVT_IDLE, &IdleCallbackHandler::OnIdle, this);
        if (FindEventHandler(this))
                Manager::Get()->GetAppWindow()->RemoveEventHandler(this);
    }

    // GetIdleCallackQueue ptr
    // ----------------------------------------------------------------------------
    std::deque<AsyncMethodCallEvent*>* GetIdleCallbackQueue()
        { return &m_AsyncMethodCallQueue; }
    // ----------------------------------------------------------------------------

    // ----------------------------------------------------------------------------
    void ClearIdleCallbacks()
    // ----------------------------------------------------------------------------
    {
        m_AsyncMethodCallQueue.clear();
    }
    // ----------------------------------------------------------------------------
    // Method callback with no paraeters
    // ----------------------------------------------------------------------------
    template <typename TP, typename T>
    void QueueCallback(TP* thisptr, void (T::*method)())
    {
        //-        QueueEvent(
        //-            new AsyncMethodCallEvent1<T, T1>(
        //-                static_cast<T*>(this), method, x1)
        //-        );
        AsyncMethodCallEvent* pCallBackEvent = new AsyncMethodCallEvent0<T>(
                                                    static_cast<T*>(thisptr), method) ;
        m_AsyncMethodCallQueue.push_back(pCallBackEvent);
    }

    // Notice that we use P1 and not T1 for the parameter to allow passing
    // parameters that are convertible to the type taken by the method
    // instead of being exactly the same, to be closer to the usual method call
    // semantics.
    // ----------------------------------------------------------------------------
    // Call back with one parameter
    // ----------------------------------------------------------------------------
    template <typename TP, typename T, typename T1, typename P1>
    void QueueCallback(TP* thisptr, void (T::*method)(T1 x1), P1 x1)
    {
        //        QueueEvent(
        //            new AsyncMethodCallEvent1<T, T1>(
        //                static_cast<T*>(this), method, x1)
        //        );
        AsyncMethodCallEvent* pCallBackEvent = new AsyncMethodCallEvent1<T, T1>(
                                                    static_cast<TP*>(thisptr), method, x1) ;
        m_AsyncMethodCallQueue.push_back(pCallBackEvent);
    }
    // ----------------------------------------------------------------------------
    // Call back with two parameters
    // ----------------------------------------------------------------------------
    template <typename TP, typename T, typename T1, typename T2, typename P1, typename P2>
    void QueueCallback(TP* thisptr, void (T::*method)(T1 x1, T2 x2), P1 x1, P2 x2)
    {
        AsyncMethodCallEvent* pCallBackEvent = new AsyncMethodCallEvent2<T, T1, T2>(
                                                    static_cast<T*>(thisptr), method, x1, x2);
        m_AsyncMethodCallQueue.push_back(pCallBackEvent);
    }
//    // ----------------------------------------------------------------------------
//    // Call back a functor (does not compile when not in same class as caller
//    // ----------------------------------------------------------------------------
//    template <typename T>
//    void QueueCallback(const T& fn)
//    {
//        //-QueueEvent(new wxAsyncMethodCallEventFunctor<T>(this, fn));
//        AsyncMethodCallEventFunctor<T>* pCallBackEvent = (new AsyncMethodCallEventFunctor<T>(this, fn));
//        m_AsyncMethodCallQueue.push_back(pCallBackEvent);
//    }
    // ----------------------------------------------------------------------------
    // Call back a functor
    // ----------------------------------------------------------------------------
    template <typename T>
    void QueueCallbackFunctor(wxObject* pThis, const T& fn)
    {
        //-QueueEvent(new wxAsyncMethodCallEventFunctor<T>(this, fn));
        AsyncMethodCallEventFunctor<T>* pCallBackEvent = (new AsyncMethodCallEventFunctor<T>(pThis, fn));
        m_AsyncMethodCallQueue.push_back(pCallBackEvent);
    }

};

#endif // IDLECALLBACKHANDLER_H_INCLUDED
