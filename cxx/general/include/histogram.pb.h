// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: histogram.proto

#ifndef PROTOBUF_INCLUDED_histogram_2eproto
#define PROTOBUF_INCLUDED_histogram_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_histogram_2eproto 

namespace protobuf_histogram_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[4];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_histogram_2eproto
namespace histogram {
class Bins;
class BinsDefaultTypeInternal;
extern BinsDefaultTypeInternal _Bins_default_instance_;
class Histogram2D;
class Histogram2DDefaultTypeInternal;
extern Histogram2DDefaultTypeInternal _Histogram2D_default_instance_;
class Histogram2DList;
class Histogram2DListDefaultTypeInternal;
extern Histogram2DListDefaultTypeInternal _Histogram2DList_default_instance_;
class MetaPair;
class MetaPairDefaultTypeInternal;
extern MetaPairDefaultTypeInternal _MetaPair_default_instance_;
}  // namespace histogram
namespace google {
namespace protobuf {
template<> ::histogram::Bins* Arena::CreateMaybeMessage<::histogram::Bins>(Arena*);
template<> ::histogram::Histogram2D* Arena::CreateMaybeMessage<::histogram::Histogram2D>(Arena*);
template<> ::histogram::Histogram2DList* Arena::CreateMaybeMessage<::histogram::Histogram2DList>(Arena*);
template<> ::histogram::MetaPair* Arena::CreateMaybeMessage<::histogram::MetaPair>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace histogram {

// ===================================================================

class Histogram2DList : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:histogram.Histogram2DList) */ {
 public:
  Histogram2DList();
  virtual ~Histogram2DList();

  Histogram2DList(const Histogram2DList& from);

  inline Histogram2DList& operator=(const Histogram2DList& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Histogram2DList(Histogram2DList&& from) noexcept
    : Histogram2DList() {
    *this = ::std::move(from);
  }

  inline Histogram2DList& operator=(Histogram2DList&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Histogram2DList& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Histogram2DList* internal_default_instance() {
    return reinterpret_cast<const Histogram2DList*>(
               &_Histogram2DList_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Histogram2DList* other);
  friend void swap(Histogram2DList& a, Histogram2DList& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Histogram2DList* New() const final {
    return CreateMaybeMessage<Histogram2DList>(NULL);
  }

  Histogram2DList* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Histogram2DList>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Histogram2DList& from);
  void MergeFrom(const Histogram2DList& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Histogram2DList* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .histogram.Histogram2D histogram = 1;
  int histogram_size() const;
  void clear_histogram();
  static const int kHistogramFieldNumber = 1;
  ::histogram::Histogram2D* mutable_histogram(int index);
  ::google::protobuf::RepeatedPtrField< ::histogram::Histogram2D >*
      mutable_histogram();
  const ::histogram::Histogram2D& histogram(int index) const;
  ::histogram::Histogram2D* add_histogram();
  const ::google::protobuf::RepeatedPtrField< ::histogram::Histogram2D >&
      histogram() const;

  // @@protoc_insertion_point(class_scope:histogram.Histogram2DList)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::histogram::Histogram2D > histogram_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_histogram_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Histogram2D : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:histogram.Histogram2D) */ {
 public:
  Histogram2D();
  virtual ~Histogram2D();

  Histogram2D(const Histogram2D& from);

  inline Histogram2D& operator=(const Histogram2D& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Histogram2D(Histogram2D&& from) noexcept
    : Histogram2D() {
    *this = ::std::move(from);
  }

  inline Histogram2D& operator=(Histogram2D&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Histogram2D& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Histogram2D* internal_default_instance() {
    return reinterpret_cast<const Histogram2D*>(
               &_Histogram2D_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Histogram2D* other);
  friend void swap(Histogram2D& a, Histogram2D& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Histogram2D* New() const final {
    return CreateMaybeMessage<Histogram2D>(NULL);
  }

  Histogram2D* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Histogram2D>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Histogram2D& from);
  void MergeFrom(const Histogram2D& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Histogram2D* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated int32 data = 1 [packed = true];
  int data_size() const;
  void clear_data();
  static const int kDataFieldNumber = 1;
  ::google::protobuf::int32 data(int index) const;
  void set_data(int index, ::google::protobuf::int32 value);
  void add_data(::google::protobuf::int32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      data() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_data();

  // repeated .histogram.MetaPair meta = 4;
  int meta_size() const;
  void clear_meta();
  static const int kMetaFieldNumber = 4;
  ::histogram::MetaPair* mutable_meta(int index);
  ::google::protobuf::RepeatedPtrField< ::histogram::MetaPair >*
      mutable_meta();
  const ::histogram::MetaPair& meta(int index) const;
  ::histogram::MetaPair* add_meta();
  const ::google::protobuf::RepeatedPtrField< ::histogram::MetaPair >&
      meta() const;

  // .histogram.Bins xbins = 2;
  bool has_xbins() const;
  void clear_xbins();
  static const int kXbinsFieldNumber = 2;
  private:
  const ::histogram::Bins& _internal_xbins() const;
  public:
  const ::histogram::Bins& xbins() const;
  ::histogram::Bins* release_xbins();
  ::histogram::Bins* mutable_xbins();
  void set_allocated_xbins(::histogram::Bins* xbins);

  // .histogram.Bins ybins = 3;
  bool has_ybins() const;
  void clear_ybins();
  static const int kYbinsFieldNumber = 3;
  private:
  const ::histogram::Bins& _internal_ybins() const;
  public:
  const ::histogram::Bins& ybins() const;
  ::histogram::Bins* release_ybins();
  ::histogram::Bins* mutable_ybins();
  void set_allocated_ybins(::histogram::Bins* ybins);

  // @@protoc_insertion_point(class_scope:histogram.Histogram2D)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > data_;
  mutable int _data_cached_byte_size_;
  ::google::protobuf::RepeatedPtrField< ::histogram::MetaPair > meta_;
  ::histogram::Bins* xbins_;
  ::histogram::Bins* ybins_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_histogram_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Bins : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:histogram.Bins) */ {
 public:
  Bins();
  virtual ~Bins();

  Bins(const Bins& from);

  inline Bins& operator=(const Bins& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Bins(Bins&& from) noexcept
    : Bins() {
    *this = ::std::move(from);
  }

  inline Bins& operator=(Bins&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Bins& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Bins* internal_default_instance() {
    return reinterpret_cast<const Bins*>(
               &_Bins_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(Bins* other);
  friend void swap(Bins& a, Bins& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Bins* New() const final {
    return CreateMaybeMessage<Bins>(NULL);
  }

  Bins* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Bins>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Bins& from);
  void MergeFrom(const Bins& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Bins* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated double bins = 1 [packed = true];
  int bins_size() const;
  void clear_bins();
  static const int kBinsFieldNumber = 1;
  double bins(int index) const;
  void set_bins(int index, double value);
  void add_bins(double value);
  const ::google::protobuf::RepeatedField< double >&
      bins() const;
  ::google::protobuf::RepeatedField< double >*
      mutable_bins();

  // @@protoc_insertion_point(class_scope:histogram.Bins)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< double > bins_;
  mutable int _bins_cached_byte_size_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_histogram_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class MetaPair : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:histogram.MetaPair) */ {
 public:
  MetaPair();
  virtual ~MetaPair();

  MetaPair(const MetaPair& from);

  inline MetaPair& operator=(const MetaPair& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  MetaPair(MetaPair&& from) noexcept
    : MetaPair() {
    *this = ::std::move(from);
  }

  inline MetaPair& operator=(MetaPair&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const MetaPair& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const MetaPair* internal_default_instance() {
    return reinterpret_cast<const MetaPair*>(
               &_MetaPair_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  void Swap(MetaPair* other);
  friend void swap(MetaPair& a, MetaPair& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline MetaPair* New() const final {
    return CreateMaybeMessage<MetaPair>(NULL);
  }

  MetaPair* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<MetaPair>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const MetaPair& from);
  void MergeFrom(const MetaPair& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(MetaPair* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string key = 1;
  void clear_key();
  static const int kKeyFieldNumber = 1;
  const ::std::string& key() const;
  void set_key(const ::std::string& value);
  #if LANG_CXX11
  void set_key(::std::string&& value);
  #endif
  void set_key(const char* value);
  void set_key(const char* value, size_t size);
  ::std::string* mutable_key();
  ::std::string* release_key();
  void set_allocated_key(::std::string* key);

  // string value = 2;
  void clear_value();
  static const int kValueFieldNumber = 2;
  const ::std::string& value() const;
  void set_value(const ::std::string& value);
  #if LANG_CXX11
  void set_value(::std::string&& value);
  #endif
  void set_value(const char* value);
  void set_value(const char* value, size_t size);
  ::std::string* mutable_value();
  ::std::string* release_value();
  void set_allocated_value(::std::string* value);

  // @@protoc_insertion_point(class_scope:histogram.MetaPair)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr key_;
  ::google::protobuf::internal::ArenaStringPtr value_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_histogram_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Histogram2DList

// repeated .histogram.Histogram2D histogram = 1;
inline int Histogram2DList::histogram_size() const {
  return histogram_.size();
}
inline void Histogram2DList::clear_histogram() {
  histogram_.Clear();
}
inline ::histogram::Histogram2D* Histogram2DList::mutable_histogram(int index) {
  // @@protoc_insertion_point(field_mutable:histogram.Histogram2DList.histogram)
  return histogram_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::histogram::Histogram2D >*
Histogram2DList::mutable_histogram() {
  // @@protoc_insertion_point(field_mutable_list:histogram.Histogram2DList.histogram)
  return &histogram_;
}
inline const ::histogram::Histogram2D& Histogram2DList::histogram(int index) const {
  // @@protoc_insertion_point(field_get:histogram.Histogram2DList.histogram)
  return histogram_.Get(index);
}
inline ::histogram::Histogram2D* Histogram2DList::add_histogram() {
  // @@protoc_insertion_point(field_add:histogram.Histogram2DList.histogram)
  return histogram_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::histogram::Histogram2D >&
Histogram2DList::histogram() const {
  // @@protoc_insertion_point(field_list:histogram.Histogram2DList.histogram)
  return histogram_;
}

// -------------------------------------------------------------------

// Histogram2D

// repeated int32 data = 1 [packed = true];
inline int Histogram2D::data_size() const {
  return data_.size();
}
inline void Histogram2D::clear_data() {
  data_.Clear();
}
inline ::google::protobuf::int32 Histogram2D::data(int index) const {
  // @@protoc_insertion_point(field_get:histogram.Histogram2D.data)
  return data_.Get(index);
}
inline void Histogram2D::set_data(int index, ::google::protobuf::int32 value) {
  data_.Set(index, value);
  // @@protoc_insertion_point(field_set:histogram.Histogram2D.data)
}
inline void Histogram2D::add_data(::google::protobuf::int32 value) {
  data_.Add(value);
  // @@protoc_insertion_point(field_add:histogram.Histogram2D.data)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
Histogram2D::data() const {
  // @@protoc_insertion_point(field_list:histogram.Histogram2D.data)
  return data_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
Histogram2D::mutable_data() {
  // @@protoc_insertion_point(field_mutable_list:histogram.Histogram2D.data)
  return &data_;
}

// .histogram.Bins xbins = 2;
inline bool Histogram2D::has_xbins() const {
  return this != internal_default_instance() && xbins_ != NULL;
}
inline void Histogram2D::clear_xbins() {
  if (GetArenaNoVirtual() == NULL && xbins_ != NULL) {
    delete xbins_;
  }
  xbins_ = NULL;
}
inline const ::histogram::Bins& Histogram2D::_internal_xbins() const {
  return *xbins_;
}
inline const ::histogram::Bins& Histogram2D::xbins() const {
  const ::histogram::Bins* p = xbins_;
  // @@protoc_insertion_point(field_get:histogram.Histogram2D.xbins)
  return p != NULL ? *p : *reinterpret_cast<const ::histogram::Bins*>(
      &::histogram::_Bins_default_instance_);
}
inline ::histogram::Bins* Histogram2D::release_xbins() {
  // @@protoc_insertion_point(field_release:histogram.Histogram2D.xbins)
  
  ::histogram::Bins* temp = xbins_;
  xbins_ = NULL;
  return temp;
}
inline ::histogram::Bins* Histogram2D::mutable_xbins() {
  
  if (xbins_ == NULL) {
    auto* p = CreateMaybeMessage<::histogram::Bins>(GetArenaNoVirtual());
    xbins_ = p;
  }
  // @@protoc_insertion_point(field_mutable:histogram.Histogram2D.xbins)
  return xbins_;
}
inline void Histogram2D::set_allocated_xbins(::histogram::Bins* xbins) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete xbins_;
  }
  if (xbins) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      xbins = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, xbins, submessage_arena);
    }
    
  } else {
    
  }
  xbins_ = xbins;
  // @@protoc_insertion_point(field_set_allocated:histogram.Histogram2D.xbins)
}

// .histogram.Bins ybins = 3;
inline bool Histogram2D::has_ybins() const {
  return this != internal_default_instance() && ybins_ != NULL;
}
inline void Histogram2D::clear_ybins() {
  if (GetArenaNoVirtual() == NULL && ybins_ != NULL) {
    delete ybins_;
  }
  ybins_ = NULL;
}
inline const ::histogram::Bins& Histogram2D::_internal_ybins() const {
  return *ybins_;
}
inline const ::histogram::Bins& Histogram2D::ybins() const {
  const ::histogram::Bins* p = ybins_;
  // @@protoc_insertion_point(field_get:histogram.Histogram2D.ybins)
  return p != NULL ? *p : *reinterpret_cast<const ::histogram::Bins*>(
      &::histogram::_Bins_default_instance_);
}
inline ::histogram::Bins* Histogram2D::release_ybins() {
  // @@protoc_insertion_point(field_release:histogram.Histogram2D.ybins)
  
  ::histogram::Bins* temp = ybins_;
  ybins_ = NULL;
  return temp;
}
inline ::histogram::Bins* Histogram2D::mutable_ybins() {
  
  if (ybins_ == NULL) {
    auto* p = CreateMaybeMessage<::histogram::Bins>(GetArenaNoVirtual());
    ybins_ = p;
  }
  // @@protoc_insertion_point(field_mutable:histogram.Histogram2D.ybins)
  return ybins_;
}
inline void Histogram2D::set_allocated_ybins(::histogram::Bins* ybins) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete ybins_;
  }
  if (ybins) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      ybins = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, ybins, submessage_arena);
    }
    
  } else {
    
  }
  ybins_ = ybins;
  // @@protoc_insertion_point(field_set_allocated:histogram.Histogram2D.ybins)
}

// repeated .histogram.MetaPair meta = 4;
inline int Histogram2D::meta_size() const {
  return meta_.size();
}
inline void Histogram2D::clear_meta() {
  meta_.Clear();
}
inline ::histogram::MetaPair* Histogram2D::mutable_meta(int index) {
  // @@protoc_insertion_point(field_mutable:histogram.Histogram2D.meta)
  return meta_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::histogram::MetaPair >*
Histogram2D::mutable_meta() {
  // @@protoc_insertion_point(field_mutable_list:histogram.Histogram2D.meta)
  return &meta_;
}
inline const ::histogram::MetaPair& Histogram2D::meta(int index) const {
  // @@protoc_insertion_point(field_get:histogram.Histogram2D.meta)
  return meta_.Get(index);
}
inline ::histogram::MetaPair* Histogram2D::add_meta() {
  // @@protoc_insertion_point(field_add:histogram.Histogram2D.meta)
  return meta_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::histogram::MetaPair >&
Histogram2D::meta() const {
  // @@protoc_insertion_point(field_list:histogram.Histogram2D.meta)
  return meta_;
}

// -------------------------------------------------------------------

// Bins

// repeated double bins = 1 [packed = true];
inline int Bins::bins_size() const {
  return bins_.size();
}
inline void Bins::clear_bins() {
  bins_.Clear();
}
inline double Bins::bins(int index) const {
  // @@protoc_insertion_point(field_get:histogram.Bins.bins)
  return bins_.Get(index);
}
inline void Bins::set_bins(int index, double value) {
  bins_.Set(index, value);
  // @@protoc_insertion_point(field_set:histogram.Bins.bins)
}
inline void Bins::add_bins(double value) {
  bins_.Add(value);
  // @@protoc_insertion_point(field_add:histogram.Bins.bins)
}
inline const ::google::protobuf::RepeatedField< double >&
Bins::bins() const {
  // @@protoc_insertion_point(field_list:histogram.Bins.bins)
  return bins_;
}
inline ::google::protobuf::RepeatedField< double >*
Bins::mutable_bins() {
  // @@protoc_insertion_point(field_mutable_list:histogram.Bins.bins)
  return &bins_;
}

// -------------------------------------------------------------------

// MetaPair

// string key = 1;
inline void MetaPair::clear_key() {
  key_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MetaPair::key() const {
  // @@protoc_insertion_point(field_get:histogram.MetaPair.key)
  return key_.GetNoArena();
}
inline void MetaPair::set_key(const ::std::string& value) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:histogram.MetaPair.key)
}
#if LANG_CXX11
inline void MetaPair::set_key(::std::string&& value) {
  
  key_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:histogram.MetaPair.key)
}
#endif
inline void MetaPair::set_key(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:histogram.MetaPair.key)
}
inline void MetaPair::set_key(const char* value, size_t size) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:histogram.MetaPair.key)
}
inline ::std::string* MetaPair::mutable_key() {
  
  // @@protoc_insertion_point(field_mutable:histogram.MetaPair.key)
  return key_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MetaPair::release_key() {
  // @@protoc_insertion_point(field_release:histogram.MetaPair.key)
  
  return key_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MetaPair::set_allocated_key(::std::string* key) {
  if (key != NULL) {
    
  } else {
    
  }
  key_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), key);
  // @@protoc_insertion_point(field_set_allocated:histogram.MetaPair.key)
}

// string value = 2;
inline void MetaPair::clear_value() {
  value_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& MetaPair::value() const {
  // @@protoc_insertion_point(field_get:histogram.MetaPair.value)
  return value_.GetNoArena();
}
inline void MetaPair::set_value(const ::std::string& value) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:histogram.MetaPair.value)
}
#if LANG_CXX11
inline void MetaPair::set_value(::std::string&& value) {
  
  value_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:histogram.MetaPair.value)
}
#endif
inline void MetaPair::set_value(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:histogram.MetaPair.value)
}
inline void MetaPair::set_value(const char* value, size_t size) {
  
  value_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:histogram.MetaPair.value)
}
inline ::std::string* MetaPair::mutable_value() {
  
  // @@protoc_insertion_point(field_mutable:histogram.MetaPair.value)
  return value_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* MetaPair::release_value() {
  // @@protoc_insertion_point(field_release:histogram.MetaPair.value)
  
  return value_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void MetaPair::set_allocated_value(::std::string* value) {
  if (value != NULL) {
    
  } else {
    
  }
  value_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set_allocated:histogram.MetaPair.value)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace histogram

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_histogram_2eproto
