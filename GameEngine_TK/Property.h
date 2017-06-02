#pragma once

#ifndef __CPP_MODULE_PROPERTY_H__
#define __CPP_MODULE_PROPERTY_H__

namespace cpp_module {

	//Getterのプロパティのポリシー
	template <typename T>
	struct SimplePropertyGetter
	{
		//Getter
		static const T& Get(const T& value) { return value; }
	};

	//Setterのプロパティのポリシー
	template <typename T>
	struct SimplePropertySetter
	{
		//Setter
		static void Set(T& value, const T& var) { value = T(var); }
	};

	//プロパティの基底クラス
	template <typename T>
	class Property
	{
	public:
		Property(T& value) : _value(value) {}
		Property(const Property& ref) : _value(ref._value) {}
		virtual ~Property() {}

		Property<T>& operator = (const Property<T>& ref)
		{
			this->_value = T(ref._value);
			return *this;
		}

	protected:
		T&		_value;
	};

	//! writable property
	template <typename T, class Getter = SimplePropertyGetter<T>, class Setter = SimplePropertySetter<T>>
	class SetterProperty : public Property<T>, private Getter, private Setter
	{
	public:
		SetterProperty(T& value) : Property<T>(value) {}
		SetterProperty(const SetterProperty& ref) : Property<T>(ref) {}
		virtual ~SetterProperty() {}

	public:
		operator const T& () const { return this->Get(this->_value); }
		const T& operator -> () const { return this->Get(this->_value); }

		SetterProperty<T, Getter, Setter>& operator = (const T& var) { this->Set(this->_value, var); return *this; }
	};

	//! readonly property
	template <typename T, class Getter = SimplePropertyGetter<T>>
	class ReadOnlyProperty : public Property<T>, private Getter
	{
	public:
		ReadOnlyProperty(T& value) : Property<T>(value) {}
		ReadOnlyProperty(const ReadOnlyProperty& ref) : Property<T>(ref) {}
		virtual ~ReadOnlyProperty() {}

	public:
		operator const T& () const { return this->Get(this->_value); }
		const T& operator -> () const { return this->Get(this->_value); }
	};

}	// end of namespace cpp_module

#endif // __CPP_MODULE_PROPERTY_H__
