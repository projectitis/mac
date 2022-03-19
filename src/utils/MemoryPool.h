#pragma once
#ifndef _MAC_MEMORYPOOLH_
#define _MAC_MEMORYPOOLH_ 1

/**
 * Memory pool for object re-use
 *
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

	/**
	 * @brief Base class for memory pooled objects
	 */
	template <class T> class MemoryPool {
	public:
		/**
		 * @brief Destroy the Pool object
		 */
		virtual ~MemoryPool() {}

		/**
		 * @brief Set all object properties back to defaults ready for re-use
		 */
		virtual void reset() {}

		/**
		 * Create factory method. Uses memory pool for object re-use.
		 */
		static T* Create() {
			T* obj;
			if ( _pool ) {
				obj = _pool;
				_pool = obj->_poolNext;
			}
			else {
				obj = new T();
			}
			return obj;
		}

		/**
		 * Return this object to the pool
		 */
		void recycle() {
			#ifdef MAC_OBJECT_REUSE
			this->reset();
			this->_poolNext = _pool;
			_pool = (T*)this;
			#else
			delete (T*)this;
			#endif
		}

		/**
		 * Frees all objects in the memory pool
		 */
		static void Destroy() {
			T* obj;
			while ( *_pool ) {
				obj = _pool;
				_pool = obj->_poolNext;
				delete obj;
			}
		}

	protected:

		static T* _pool;
		T* _poolNext = nullptr;

	};

	template <class T>
	T* MemoryPool<T>::_pool = nullptr;

} // namespace

#endif