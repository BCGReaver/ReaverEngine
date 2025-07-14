/*
 * MIT License
 *
 * Copyright (c) 2024 Roberto Charreton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * In addition, any project or software that uses this library or class must include
 * the following acknowledgment in the credits:
 *
 * "This project uses software developed by Roberto Charreton and Attribute Overload."
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
#pragma once
// #include <atomic> // Para un shared_ptr robusto y thread-safe, necesitarías esto.
#include <utility> // Para std::forward en MakeShared

namespace EngineUtilities {
	/**
	 * @brief Clase TSharedPointer para manejar la gestión de memoria compartida.
	 *
	 * La clase TSharedPointer gestiona la memoria de un objeto de tipo T y lleva un
	 * recuento de referencias para permitir la compartición segura de un mismo objeto
	 * en múltiples instancias de TSharedPointer.
	 */
	template<typename T>
	class TSharedPointer
	{
	public:
		/**
		 * @brief Constructor por defecto.
		 *
		 * Inicializa el puntero y el recuento de referencias a nullptr.
		 */
		TSharedPointer() : ptr(nullptr), refCount(nullptr) {}

		/**
		 * @brief Constructor que toma un puntero crudo.
		 *
		 * @param rawPtr Puntero crudo al objeto que se va a gestionar.
		 */
		explicit TSharedPointer(T* rawPtr) : ptr(rawPtr), refCount(new int(1)) {}

		/**
		 * @brief Constructor desde un puntero crudo y un recuento de referencias.
		 * Este es CRÍTICO para dynamic_pointer_cast y static_pointer_cast
		 * ya que permite que el nuevo puntero compartido APUNTE AL MISMO CONTADOR
		 * de referencias del puntero compartido original.
		 *
		 * @param rawPtr Puntero crudo al objeto gestionado.
		 * @param existingRefCount Puntero al recuento de referencias existente.
		 */
		TSharedPointer(T* rawPtr, int* existingRefCount) : ptr(rawPtr), refCount(existingRefCount)
		{
			if (refCount)
			{
				++(*refCount);
			}
		}

		/**
		 * @brief Constructor de copia.
		 *
		 * Copia el puntero y el recuento de referencias del otro TSharedPointer y
		 * aumenta el recuento de referencias.
		 *
		 * @param other Otro objeto TSharedPointer del mismo tipo T.
		 */
		TSharedPointer(const TSharedPointer<T>& other) : ptr(other.ptr), refCount(other.refCount)
		{
			if (refCount)
			{
				++(*refCount);
			}
		}

		/**
		 * @brief Constructor de movimiento.
		 *
		 * Transfiere la propiedad del puntero y el recuento de referencias del otro
		 * TSharedPointer al nuevo objeto TSharedPointer.
		 *
		 * @param other Otro objeto TSharedPointer del mismo tipo T.
		 */
		TSharedPointer(TSharedPointer<T>&& other) noexcept : ptr(other.ptr), refCount(other.refCount)
		{
			other.ptr = nullptr;
			other.refCount = nullptr;
		}

		/**
		 * @brief Operador de asignación de copia.
		 *
		 * Libera el objeto actual, copia el puntero y el recuento de referencias del otro
		 * TSharedPointer, y aumenta el recuento de referencias.
		 *
		 * @param other Otro objeto TSharedPointer del mismo tipo T.
		 * @return Referencia al objeto TSharedPointer actual.
		 */
		TSharedPointer<T>& operator=(const TSharedPointer<T>& other)
		{
			if (this != &other)
			{
				// Disminuir el recuento de referencias del objeto actual
				if (refCount)
				{
					--(*refCount);
					if (*refCount == 0)
					{
						delete ptr;
						delete refCount;
					}
				}
				// Copiar datos del otro puntero compartido
				ptr = other.ptr;
				refCount = other.refCount;
				if (refCount)
				{
					++(*refCount);
				}
			}
			return *this;
		}

		/**
		 * @brief Operador de asignación de movimiento.
		 *
		 * Libera el objeto actual, transfiere la propiedad del puntero y el recuento de
		 * referencias del otro TSharedPointer al actual.
		 *
		 * @param other Otro objeto TSharedPointer del mismo tipo T.
		 * @return Referencia al objeto TSharedPointer actual.
		 */
		TSharedPointer<T>& operator=(TSharedPointer<T>&& other) noexcept
		{
			if (this != &other)
			{
				// Liberar el objeto actual
				if (refCount)
				{
					--(*refCount);
					if (*refCount == 0)
					{
						delete ptr;
						delete refCount;
					}
				}
				// Transferir los datos del otro puntero compartido
				ptr = other.ptr;
				refCount = other.refCount;
				other.ptr = nullptr;
				other.refCount = nullptr;
			}
			return *this;
		}

		/**
		 * @brief Destructor.
		 *
		 * Disminuye el recuento de referencias y libera la memoria del objeto
		 * gestionado si el recuento de referencias llega a cero.
		 */
		~TSharedPointer()
		{
			if (refCount)
			{
				--(*refCount);
				if (*refCount == 0)
				{
					delete ptr;
					delete refCount;
				}
			}
		}

		/**
		 * @brief Operador de desreferenciación.
		 *
		 * @return Referencia al objeto gestionado.
		 */
		T& operator*() const { return *ptr; }

		/**
		 * @brief Operador de acceso a miembros.
		 *
		 * @return Puntero al objeto gestionado.
		 */
		T* operator->() const { return ptr; }

		/**
		 * @brief Obtener el puntero crudo.
		 *
		 * @return Puntero crudo al objeto gestionado.
		 */
		T* get() const { return ptr; }

		/**
		 * @brief Comprobar si el puntero es nulo.
		 *
		 * @return true si el puntero es nulo, false en caso contrario.
		 */
		bool isNull() const { return ptr == nullptr; }

		/**
		 * @brief Operador de conversión a booleano.
		 * Permite que TSharedPointer sea usado directamente en contextos booleanos (ej. if (ptr)).
		 * @return true si el puntero interno no es nulo, false en caso contrario.
		 */
		explicit operator bool() const {
			return ptr != nullptr;
		}

		/**
		 * @brief Realiza un dynamic_cast a un nuevo TSharedPointer.
		 * Es crucial que este método cree un nuevo TSharedPointer que comparta el mismo refCount.
		 * @tparam U El tipo al que se desea castear. Debe derivar del tipo actual T.
		 * @return Un TSharedPointer<U> si el cast es exitoso, un TSharedPointer<U> nulo en caso contrario.
		 */
		template<typename U>
		TSharedPointer<U> dynamic_pointer_cast() const {
			U* castedPtr = dynamic_cast<U*>(ptr);
			if (castedPtr) {
				// Si el cast es exitoso, crea un nuevo TSharedPointer que apunte al mismo recurso
				// y comparta el mismo contador de referencias.
				return TSharedPointer<U>(castedPtr, refCount);
			}
			return TSharedPointer<U>(); // Retorna un puntero nulo si el cast falla
		}

		/**
		 * @brief Realiza un static_cast a un nuevo TSharedPointer.
		 * @tparam U El tipo al que se desea castear.
		 * @return Un TSharedPointer<U> con el puntero casteado.
		 */
		template<typename U>
		TSharedPointer<U> static_pointer_cast() const {
			U* castedPtr = static_cast<U*>(ptr);
			// static_cast siempre es exitoso si las jerarquías son correctas.
			return TSharedPointer<U>(castedPtr, refCount); // Comparte el mismo contador de referencias
		}


	public: // Considera si estos deberían ser públicos o privados con getters/setters si no quieres acceso directo.
		T* ptr;
		int* refCount; ///< Puntero al recuento de referencias.

		/**
		 * @brief Método swap.
		 *
		 * Intercambia los datos de dos objetos TSharedPointer.
		 *
		 * @param other Otro objeto TSharedPointer del mismo tipo T.
		 */
		void swap(TSharedPointer<T>& other) noexcept
		{
			T* tempPtr = other.ptr;
			int* tempRefCount = other.refCount;

			other.ptr = this->ptr;
			other.refCount = this->refCount;

			this->ptr = tempPtr;
			this->refCount = tempRefCount;
		}

		/**
		 * @brief Libera el objeto actual y opcionalmente asigna un nuevo objeto.
		 *
		 * @param newPtr Nuevo puntero crudo al objeto que se va a gestionar (por defecto es nullptr).
		 */
		void reset(T* newPtr = nullptr)
		{
			// Disminuir el recuento de referencias del objeto actual
			if (refCount)
			{
				--(*refCount);
				if (*refCount == 0)
				{
					delete ptr;
					delete refCount;
				}
			}

			// Si newPtr es nullptr, asignar nullptr al puntero y recuento de referencias
			if (newPtr == nullptr)
			{
				ptr = nullptr;
				refCount = nullptr;
			}
			else
			{
				// Asignar nuevo objeto y manejar el recuento de referencias
				ptr = newPtr;
				refCount = new int(1);
			}
		}
	};

	/**
	 * @brief Función de utilidad para crear un TSharedPointer.
	 *
	 * @tparam T Tipo del objeto gestionado.
	 * @tparam Args Tipos de los argumentos del constructor del objeto gestionado.
	 * @param args Argumentos del constructor del objeto gestionado.
	 * @return Un objeto TSharedPointer gestionando un nuevo objeto de tipo T.
	 */
	template<typename T, typename... Args>
	TSharedPointer<T> MakeShared(Args&&... args)
	{
		return TSharedPointer<T>(new T(std::forward<Args>(args)...));
	}
}