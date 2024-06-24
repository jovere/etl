/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "unit_test_framework.h"

#include "etl/observer.h"

namespace
{
  //*****************************************************************************
  // Notification1
  //*****************************************************************************
  struct Notification1
  {
    // constructor
    Notification1() = default;
    // constructor
    explicit Notification1(int data) : data(data) {}
    // copy constructor
    Notification1(const Notification1&) = default;
    // move constructor
    Notification1(Notification1&& other) noexcept
      : data(std::exchange(other.data, -1)){};
    // copy assignment
    Notification1& operator = (const Notification1&) = default;
    // move assignment
    Notification1& operator = (Notification1&& other) noexcept
    {
      std::swap(data, other.data);
      return *this;
    }
    virtual ~Notification1() = default; // destructor

    int get_data() const { return data; }
  private:
    int data = 24;
  };

  //*****************************************************************************
  // Notification2
  //*****************************************************************************
  struct Notification2
  {
    // constructor
    Notification2() = default;
    // constructor
    explicit Notification2(int data) : data(data) {}
    // copy constructor
    Notification2(const Notification2&) = delete;
    // move constructor
    Notification2(Notification2&& other) noexcept
      : data(std::exchange(other.data, -1)){};
    // copy assignment
    Notification2& operator = (const Notification2&) = delete;
    // move assignment
    Notification2& operator = (Notification2&& other) noexcept
    {
      std::swap(data, other.data);
      return *this;
    }
    virtual ~Notification2() = default; // destructor

    int get_data() const { return data; }
  private:
    int data = 48;
  };

  //*****************************************************************************
  // Notification3
  //*****************************************************************************
  struct Notification3
  {
    // constructor
    Notification3() = default;
    // constructor
    explicit Notification3(int data) : data(data) {}
    // copy constructor
    Notification3(const Notification3&) = delete;
    // move constructor
    Notification3(Notification3&& other) noexcept
      : data(std::exchange(other.data, -1)){};
    // copy assignment
    Notification3& operator = (const Notification3&) = delete;
    // move assignment
    Notification3& operator = (Notification3&& other) noexcept
    {
      std::swap(data, other.data);
      return *this;
    }
    virtual ~Notification3() = default; // destructor

    int get_data() const { return data; }
  private:
    int data = 42;
  };

  //*****************************************************************************
  // Generic notification.
  //*****************************************************************************
  template <const int ID>
  struct Notification
  {
  };

  //*****************************************************************************
  // The observer base type.
  // Declare what notifications you want to observe and how they are passed to 'notification'.
  // The Notification1 is passed by value.
  // The Notification2 is passed by reference.
  // The Notification3 is passed by const reference.
  //*****************************************************************************
  typedef etl::observer<Notification1, Notification2&, const Notification3&> ObserverType;
}

//*****************************************************************************
// The concrete observable 1 class.
//*****************************************************************************
class Observable1 : public etl::observable<ObserverType, 2>
{
public:

	Notification1 data1;
	Notification2 data2;
  Notification1& data3 = data1;

  //*********************************
  // Notify all of the observers.
  //*********************************
	void send_notifications()
	{
		notify_observers(data3);
    notify_observers(data2);
	}
};

//*****************************************************************************
// The concrete observable 2 class.
//*****************************************************************************
class Observable2 : public etl::observable<ObserverType, 2>
{
public:

	Notification3 data3;

  //*********************************
  // Notify all of the observers.
  //*********************************
	void send_notifications()
	{
    notify_observers(data3);
	}
};

//*****************************************************************************
// The concrete observable 3 class.
//*****************************************************************************
class Observable3 : public etl::observable<ObserverType, 3>
{
public:

  //*********************************
  // Notify all of the observers.
  //*********************************
  void send_notifications()
  {
    notify_observers(Notification1{34});
    notify_observers(Notification2{97});
  }
};

//*****************************************************************************
// The observer type.
// If any one of the overloads is missing or a parameter declaration is incorrect
// then the class will be 'abstract' and will not compile.
//*****************************************************************************
class TestObserver : public ObserverType
{
public:

  TestObserver()
    : data1_count(0),
      data2_count(0),
      data3_count(0)
  {
  }

  //*******************************************
  // Notification1 is passed by value.
  //*******************************************
  void notification(Notification1 /*data1*/)
	{
    ++data1_count;
	}

  //*******************************************
  // Notification2 is passed by reference.
  //*******************************************
  void notification(Notification2& /*data2*/)
	{
    ++data2_count;
	}

  //*******************************************
	// Notification3 is passed by const reference.
  //*******************************************
  void notification(const Notification3& /*data3*/)
	{
    ++data3_count;
	}

  int data1_count;
  int data2_count;
  int data3_count;
};

//*****************************************************************************
// The observer type.
// If any one of the overloads is missing or a parameter declaration is incorrect
// then the class will be 'abstract' and will not compile.
//*****************************************************************************
class TestObserver2 : public ObserverType
{
public:

  TestObserver2()
  {
  }

  //*******************************************
  // Notification1 is passed by value.
  //*******************************************
  void notification(Notification1 data1)
  {
    m_data1 = data1;
  }

  //*******************************************
  // Notification2 is passed by reference.
  //*******************************************
  void notification(Notification2& data2)
  {
    m_notification2data = data2.get_data();
  }

  //*******************************************
  // Notification3 is passed by const reference.
  //*******************************************
  void notification(const Notification3& /*data3*/)
  {
  }

  Notification1 m_data1{};
  int m_notification2data{1234};
};

namespace
{
  SUITE(test_observer)
  {
    //*************************************************************************
    TEST(test_2_observables_2_observers_3_notifications)
    {
      // The observable objects.
      Observable1 observable1;
      Observable2 observable2;

      // The observer objects.
      TestObserver observer1;
      TestObserver observer2;

      observable1.add_observer(observer1);

      // Send the notifications.
      observable1.send_notifications(); // Updates data1 & data2.

      CHECK_EQUAL(1, observer1.data1_count);
      CHECK_EQUAL(1, observer1.data2_count);
      CHECK_EQUAL(0, observer1.data3_count);

      CHECK_EQUAL(0, observer2.data1_count);
      CHECK_EQUAL(0, observer2.data2_count);
      CHECK_EQUAL(0, observer2.data3_count);

      observable2.send_notifications(); // Updates data3. observeable2 has no observers yet.

      CHECK_EQUAL(1, observer1.data1_count);
      CHECK_EQUAL(1, observer1.data2_count);
      CHECK_EQUAL(0, observer1.data3_count);

      CHECK_EQUAL(0, observer2.data1_count);
      CHECK_EQUAL(0, observer2.data2_count);
      CHECK_EQUAL(0, observer2.data3_count);

      // Add an observer to both.
      observable1.add_observer(observer2);
      observable2.add_observer(observer2);

      // Send the notifications.
      observable1.send_notifications(); // Updates data1 & data2.

      CHECK_EQUAL(2, observer1.data1_count);
      CHECK_EQUAL(2, observer1.data2_count);
      CHECK_EQUAL(0, observer1.data3_count);

      CHECK_EQUAL(1, observer2.data1_count);
      CHECK_EQUAL(1, observer2.data2_count);
      CHECK_EQUAL(0, observer2.data3_count);

      observable2.send_notifications(); // Updates data3.

      CHECK_EQUAL(2, observer1.data1_count);
      CHECK_EQUAL(2, observer1.data2_count);
      CHECK_EQUAL(0, observer1.data3_count);

      CHECK_EQUAL(1, observer2.data1_count);
      CHECK_EQUAL(1, observer2.data2_count);
      CHECK_EQUAL(1, observer2.data3_count);

      observable1.remove_observer(observer1);

      // Send the notifications.
      observable1.send_notifications(); // Updates data1 & data2.

      CHECK_EQUAL(2, observer1.data1_count);
      CHECK_EQUAL(2, observer1.data2_count);
      CHECK_EQUAL(0, observer1.data3_count);

      CHECK_EQUAL(2, observer2.data1_count);
      CHECK_EQUAL(2, observer2.data2_count);
      CHECK_EQUAL(1, observer2.data3_count);

      observable2.send_notifications(); // Updates data3.

      CHECK_EQUAL(2, observer1.data1_count);
      CHECK_EQUAL(2, observer1.data2_count);
      CHECK_EQUAL(0, observer1.data3_count);

      CHECK_EQUAL(2, observer2.data1_count);
      CHECK_EQUAL(2, observer2.data2_count);
      CHECK_EQUAL(2, observer2.data3_count);
    }

    //*************************************************************************
    TEST(test_observable_2_observers_enable_disable)
    {
      // The observable objects.
      Observable1 observable1;

      // The observer objects.
      TestObserver observer1;
      TestObserver observer2;

      observable1.add_observer(observer1);
      observable1.add_observer(observer2);

      // Send the notifications.
      observable1.send_notifications();

      CHECK_EQUAL(1, observer1.data1_count);
      CHECK_EQUAL(1, observer2.data1_count);

      // Disable observer1. Send the notifications.
      observable1.disable_observer(observer1);
      observable1.send_notifications();

      CHECK_EQUAL(1, observer1.data1_count);
      CHECK_EQUAL(2, observer2.data1_count);

      // Disable observer2. Send the notifications.
      observable1.enable_observer(observer2, false);
      observable1.send_notifications();

      CHECK_EQUAL(1, observer1.data1_count);
      CHECK_EQUAL(2, observer2.data1_count);

      // Enable observer1. Send the notifications.
      observable1.enable_observer(observer1);
      observable1.send_notifications();

      CHECK_EQUAL(2, observer1.data1_count);
      CHECK_EQUAL(2, observer2.data1_count);

      // Enable observer2. Send the notifications.
      observable1.enable_observer(observer2, true);
      observable1.send_notifications();

      CHECK_EQUAL(3, observer1.data1_count);
      CHECK_EQUAL(3, observer2.data1_count);
    }

    TEST(test_that_data_is_not_moved_when_sent_to_multiple_observers)
    {
      // The observable objects.
      Observable3 observable3;

      // The observer objects.
      TestObserver2 observer1;
      TestObserver2 observer2;

      observable3.add_observer(observer1);
      observable3.add_observer(observer2);

      observable3.send_notifications();

      Notification1 data{34};
      CHECK_EQUAL(data.get_data(), observer1.m_data1.get_data());
      CHECK_EQUAL(data.get_data(), observer2.m_data1.get_data());
      Notification2 data2{97};
      CHECK_EQUAL(data2.get_data(), observer1.m_notification2data);
      CHECK_EQUAL(data2.get_data(), observer2.m_notification2data);
    }

    //*************************************************************************
    TEST(test_8_notifications)
    {
      typedef etl::observer<Notification<1>, Notification<2>, Notification<3>, Notification<4>, Notification<5>, Notification<6>, Notification<7>, Notification<8> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_7_notifications)
    {
      typedef etl::observer<Notification<1>, Notification<2>, Notification<3>, Notification<4>, Notification<5>, Notification<6>, Notification<7> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_6_notifications)
    {
      typedef etl::observer<Notification<1>, Notification<2>, Notification<3>, Notification<4>, Notification<5>, Notification<6> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_5_notifications)
    {
      typedef etl::observer<Notification<1>, Notification<2>, Notification<3>, Notification<4>, Notification<5> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_4_notifications)
    {
      typedef etl::observer<Notification<1>, Notification<2>, Notification<3>, Notification<4> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_3_notifications)
    {
      typedef etl::observer<Notification<1>, Notification<2>, Notification<3> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_2_notifications)
    {
      typedef etl::observer<Notification<1>, Notification<2> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_1_notification)
    {
      typedef etl::observer<Notification<1> > Observer;

      class Observable : public etl::observable<Observer, 1>
      {
      };

      // This test just needs to compile without errors.
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_observer_list)
    {
      class Observer : public etl::observer<Notification1>
      {
        void notification(Notification1) {}
      };

      class Observable : public etl::observable<Observer, 4>
      {
      };

      Observable observable;

      Observer observer1;
      Observer observer2;
      Observer observer3;
      Observer observer4;
      Observer observer5;

      observable.add_observer(observer1);
      CHECK_EQUAL(size_t(1UL), observable.number_of_observers());

      observable.add_observer(observer2);
      CHECK_EQUAL(size_t(2UL), observable.number_of_observers());

      observable.add_observer(observer3);
      CHECK_EQUAL(size_t(3UL), observable.number_of_observers());

      observable.add_observer(observer2);
      CHECK_EQUAL(size_t(3UL), observable.number_of_observers());

      observable.add_observer(observer4);
      CHECK_EQUAL(size_t(4UL), observable.number_of_observers());

      CHECK_THROW(observable.add_observer(observer5), etl::observer_list_full);

      CHECK(observable.remove_observer(observer3));
      CHECK_EQUAL(size_t(3UL), observable.number_of_observers());

      // Try again.
      CHECK(!observable.remove_observer(observer3));
      CHECK_EQUAL(size_t(3UL), observable.number_of_observers());

      observable.clear_observers();
      CHECK_EQUAL(size_t(0UL), observable.number_of_observers());
    }
  }
}

