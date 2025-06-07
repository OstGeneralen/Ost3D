// Ost3D::Core - Kasper "OstGeneralen" Esbjornsson - 2025

#pragma once
#include <Ost3D/Core/Public/TypeDefs.h>

#include <concepts>
#include <type_traits>

// ------------------------------------------------------------

namespace ost
{
	// ------------------------------------------------------------
	// The TTypedID template
	// ------------------------------------------------------------
	template< 
		typename T_ID,			// The underlying ID type (i.e uint16)
		T_ID V_UNSET,			// Value of T_ID type representing default/invalid/unset
		typename UNIQUE_TAG		// Phantom type tag to type safe the specific ID type
	>
	struct TTypedID
	{
		using ID_t = T_ID;

		TTypedID() noexcept : _id{ V_UNSET } {}
		TTypedID(T_ID idVal) noexcept : _id(idVal) {}
		TTypedID(const TTypedID& o) noexcept : _id(o._id) {}
		TTypedID(TTypedID&& o) noexcept : _id(o._id) { o._id = V_UNSET; }

		void Invalidate() { _id = V_UNSET; }
		bool Valid() const { return _id != V_UNSET; }

		TTypedID& operator=(const TTypedID& rhs) { _id = rhs._id; return *this; }
		TTypedID& operator=(TTypedID&& rhs) noexcept { _id = rhs._id; rhs._id = V_UNSET; return *this; }

		bool operator==(const TTypedID& r) const { return _id == r._id; }
		bool operator<(const TTypedID& r) const { return _id < r._id; }
		bool operator<=(const TTypedID& r) const { return _id <= r._id; }

		explicit operator T_ID() const { return _id; }
	private:
		T_ID _id;
	};

	// ------------------------------------------------------------
	// Internal namespace for default supported id types
	// ------------------------------------------------------------
	namespace id_types
	{
		template<typename T>
		struct default_id_type
		{
			constexpr static T UnsetValue{};
		};

		template<> struct default_id_type<uint8>
		{
			constexpr static uint8 UnsetValue = 0xFAu;
		};
		template<> struct default_id_type<uint16>
		{
			constexpr static uint8 UnsetValue = 0xFAFAu;
		};
		template<> struct default_id_type<uint32>
		{
			constexpr static uint8 UnsetValue = 0xFAFAFAFAu;
		};
		template<> struct default_id_type<uint64>
		{
			constexpr static uint8 UnsetValue = 0xFAFAFAFAFAFAFAFAu;
		};
	}


	// ------------------------------------------------------------
	// Built-in ID types of uint varieties
	// ------------------------------------------------------------
	template<typename TGhost, uint8 v_unset = id_types::default_id_type<uint8>::UnsetValue>		
	using IDType_uint8 = TTypedID<uint8, v_unset, TGhost>;
	
	template<typename TGhost, uint16 v_unset = id_types::default_id_type<uint16>::UnsetValue>	
	using IDType_uint16 = TTypedID<uint16, v_unset, TGhost>;
	
	template<typename TGhost, uint32 v_unset = id_types::default_id_type<uint32>::UnsetValue>	
	using IDType_uint32 = TTypedID<uint32, v_unset, TGhost>;
	
	template<typename TGhost, uint64 v_unset = id_types::default_id_type<uint64>::UnsetValue>	
	using IDType_uint64 = TTypedID<uint32, v_unset, TGhost>;
}

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------