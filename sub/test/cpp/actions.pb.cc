// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: actions.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "actions.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace msg {
class Action_TestDefaultTypeInternal {
public:
 ::google::protobuf::internal::ExplicitlyConstructed<Action_Test>
     _instance;
} _Action_Test_default_instance_;
class ActionDefaultTypeInternal {
public:
 ::google::protobuf::internal::ExplicitlyConstructed<Action>
     _instance;
  const ::msg::Action_Test* test_;
} _Action_default_instance_;
class ActionsDefaultTypeInternal {
public:
 ::google::protobuf::internal::ExplicitlyConstructed<Actions>
     _instance;
} _Actions_default_instance_;

namespace protobuf_actions_2eproto {


namespace {

::google::protobuf::Metadata file_level_metadata[3];

}  // namespace

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTableField
    const TableStruct::entries[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  {0, 0, 0, ::google::protobuf::internal::kInvalidMask, 0, 0},
};

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::AuxillaryParseTableField
    const TableStruct::aux[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ::google::protobuf::internal::AuxillaryParseTableField(),
};
PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTable const
    TableStruct::schema[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { NULL, NULL, 0, -1, -1, -1, -1, NULL, false },
  { NULL, NULL, 0, -1, -1, -1, -1, NULL, false },
  { NULL, NULL, 0, -1, -1, -1, -1, NULL, false },
};

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Action_Test, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Action, _internal_metadata_),
  ~0u,  // no _extensions_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Action, _oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  offsetof(ActionDefaultTypeInternal, test_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Action, data_),
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Actions, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Actions, action_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(Action_Test)},
  { 5, -1, sizeof(Action)},
  { 12, -1, sizeof(Actions)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&_Action_Test_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&_Action_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&_Actions_default_instance_),
};

namespace {

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "actions.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 3);
}

}  // namespace
void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  _Action_Test_default_instance_._instance.DefaultConstruct();
  ::google::protobuf::internal::OnShutdownDestroyMessage(
      &_Action_Test_default_instance_);_Action_default_instance_._instance.DefaultConstruct();
  ::google::protobuf::internal::OnShutdownDestroyMessage(
      &_Action_default_instance_);_Actions_default_instance_._instance.DefaultConstruct();
  ::google::protobuf::internal::OnShutdownDestroyMessage(
      &_Actions_default_instance_);_Action_default_instance_.test_ = const_cast< ::msg::Action_Test*>(
      ::msg::Action_Test::internal_default_instance());
}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
namespace {
void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\ractions.proto\022\003msg\":\n\006Action\022 \n\004test\030\001"
      " \001(\0132\020.msg.Action.TestH\000\032\006\n\004TestB\006\n\004data"
      "\"&\n\007Actions\022\033\n\006action\030\001 \003(\0132\013.msg.Action"
      "B\003\370\001\001b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 133);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "actions.proto", &protobuf_RegisterTypes);
}
} // anonymous namespace

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;

}  // namespace protobuf_actions_2eproto


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Action_Test::Action_Test()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_actions_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:msg.Action.Test)
}
Action_Test::Action_Test(::google::protobuf::Arena* arena)
  : ::google::protobuf::Message(),
  _internal_metadata_(arena) {
  protobuf_actions_2eproto::InitDefaults();
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:msg.Action.Test)
}
Action_Test::Action_Test(const Action_Test& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:msg.Action.Test)
}

void Action_Test::SharedCtor() {
  _cached_size_ = 0;
}

Action_Test::~Action_Test() {
  // @@protoc_insertion_point(destructor:msg.Action.Test)
  SharedDtor();
}

void Action_Test::SharedDtor() {
  ::google::protobuf::Arena* arena = GetArenaNoVirtual();
  GOOGLE_DCHECK(arena == NULL);
  if (arena != NULL) {
    return;
  }

}

void Action_Test::ArenaDtor(void* object) {
  Action_Test* _this = reinterpret_cast< Action_Test* >(object);
  (void)_this;
}
void Action_Test::RegisterArenaDtor(::google::protobuf::Arena* arena) {
}
void Action_Test::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Action_Test::descriptor() {
  protobuf_actions_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_actions_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Action_Test& Action_Test::default_instance() {
  protobuf_actions_2eproto::InitDefaults();
  return *internal_default_instance();
}

Action_Test* Action_Test::New(::google::protobuf::Arena* arena) const {
  return ::google::protobuf::Arena::CreateMessage<Action_Test>(arena);
}

void Action_Test::Clear() {
// @@protoc_insertion_point(message_clear_start:msg.Action.Test)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear();
}

bool Action_Test::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:msg.Action.Test)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, _internal_metadata_.mutable_unknown_fields()));
  }
success:
  // @@protoc_insertion_point(parse_success:msg.Action.Test)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:msg.Action.Test)
  return false;
#undef DO_
}

void Action_Test::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:msg.Action.Test)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:msg.Action.Test)
}

::google::protobuf::uint8* Action_Test::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:msg.Action.Test)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:msg.Action.Test)
  return target;
}

size_t Action_Test::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:msg.Action.Test)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Action_Test::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:msg.Action.Test)
  GOOGLE_DCHECK_NE(&from, this);
  const Action_Test* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Action_Test>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:msg.Action.Test)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:msg.Action.Test)
    MergeFrom(*source);
  }
}

void Action_Test::MergeFrom(const Action_Test& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:msg.Action.Test)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void Action_Test::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:msg.Action.Test)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Action_Test::CopyFrom(const Action_Test& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:msg.Action.Test)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Action_Test::IsInitialized() const {
  return true;
}

void Action_Test::Swap(Action_Test* other) {
  if (other == this) return;
  if (GetArenaNoVirtual() == other->GetArenaNoVirtual()) {
    InternalSwap(other);
  } else {
    Action_Test* temp = New(GetArenaNoVirtual());
    temp->MergeFrom(*other);
    other->CopyFrom(*this);
    InternalSwap(temp);
    if (GetArenaNoVirtual() == NULL) {
      delete temp;
    }
  }
}
void Action_Test::UnsafeArenaSwap(Action_Test* other) {
  if (other == this) return;
  GOOGLE_DCHECK(GetArenaNoVirtual() == other->GetArenaNoVirtual());
  InternalSwap(other);
}
void Action_Test::InternalSwap(Action_Test* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Action_Test::GetMetadata() const {
  protobuf_actions_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_actions_2eproto::file_level_metadata[kIndexInFileMessages];
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Action_Test

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Action::kTestFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Action::Action()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_actions_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:msg.Action)
}
Action::Action(::google::protobuf::Arena* arena)
  : ::google::protobuf::Message(),
  _internal_metadata_(arena) {
  protobuf_actions_2eproto::InitDefaults();
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:msg.Action)
}
Action::Action(const Action& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  clear_has_data();
  switch (from.data_case()) {
    case kTest: {
      mutable_test()->::msg::Action_Test::MergeFrom(from.test());
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:msg.Action)
}

void Action::SharedCtor() {
  clear_has_data();
  _cached_size_ = 0;
}

Action::~Action() {
  // @@protoc_insertion_point(destructor:msg.Action)
  SharedDtor();
}

void Action::SharedDtor() {
  ::google::protobuf::Arena* arena = GetArenaNoVirtual();
  GOOGLE_DCHECK(arena == NULL);
  if (arena != NULL) {
    return;
  }

  if (has_data()) {
    clear_data();
  }
}

void Action::ArenaDtor(void* object) {
  Action* _this = reinterpret_cast< Action* >(object);
  (void)_this;
}
void Action::RegisterArenaDtor(::google::protobuf::Arena* arena) {
}
void Action::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Action::descriptor() {
  protobuf_actions_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_actions_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Action& Action::default_instance() {
  protobuf_actions_2eproto::InitDefaults();
  return *internal_default_instance();
}

Action* Action::New(::google::protobuf::Arena* arena) const {
  return ::google::protobuf::Arena::CreateMessage<Action>(arena);
}

void Action::clear_data() {
// @@protoc_insertion_point(one_of_clear_start:msg.Action)
  switch (data_case()) {
    case kTest: {
      if (GetArenaNoVirtual() == NULL) {
        delete data_.test_;
      }
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  _oneof_case_[0] = DATA_NOT_SET;
}


void Action::Clear() {
// @@protoc_insertion_point(message_clear_start:msg.Action)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  clear_data();
  _internal_metadata_.Clear();
}

bool Action::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:msg.Action)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // .msg.Action.Test test = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_test()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:msg.Action)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:msg.Action)
  return false;
#undef DO_
}

void Action::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:msg.Action)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .msg.Action.Test test = 1;
  if (has_test()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, *data_.test_, output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:msg.Action)
}

::google::protobuf::uint8* Action::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:msg.Action)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .msg.Action.Test test = 1;
  if (has_test()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        1, *data_.test_, deterministic, target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:msg.Action)
  return target;
}

size_t Action::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:msg.Action)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  switch (data_case()) {
    // .msg.Action.Test test = 1;
    case kTest: {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *data_.test_);
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Action::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:msg.Action)
  GOOGLE_DCHECK_NE(&from, this);
  const Action* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Action>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:msg.Action)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:msg.Action)
    MergeFrom(*source);
  }
}

void Action::MergeFrom(const Action& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:msg.Action)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  switch (from.data_case()) {
    case kTest: {
      mutable_test()->::msg::Action_Test::MergeFrom(from.test());
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
}

void Action::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:msg.Action)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Action::CopyFrom(const Action& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:msg.Action)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Action::IsInitialized() const {
  return true;
}

void Action::Swap(Action* other) {
  if (other == this) return;
  if (GetArenaNoVirtual() == other->GetArenaNoVirtual()) {
    InternalSwap(other);
  } else {
    Action* temp = New(GetArenaNoVirtual());
    temp->MergeFrom(*other);
    other->CopyFrom(*this);
    InternalSwap(temp);
    if (GetArenaNoVirtual() == NULL) {
      delete temp;
    }
  }
}
void Action::UnsafeArenaSwap(Action* other) {
  if (other == this) return;
  GOOGLE_DCHECK(GetArenaNoVirtual() == other->GetArenaNoVirtual());
  InternalSwap(other);
}
void Action::InternalSwap(Action* other) {
  using std::swap;
  swap(data_, other->data_);
  swap(_oneof_case_[0], other->_oneof_case_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Action::GetMetadata() const {
  protobuf_actions_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_actions_2eproto::file_level_metadata[kIndexInFileMessages];
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Action

// .msg.Action.Test test = 1;
bool Action::has_test() const {
  return data_case() == kTest;
}
void Action::set_has_test() {
  _oneof_case_[0] = kTest;
}
void Action::clear_test() {
  if (has_test()) {
    if (GetArenaNoVirtual() == NULL) {
      delete data_.test_;
    }
    clear_has_data();
  }
}
 const ::msg::Action_Test& Action::test() const {
  // @@protoc_insertion_point(field_get:msg.Action.test)
  return has_test()
      ? *data_.test_
      : ::msg::Action_Test::default_instance();
}
::msg::Action_Test* Action::mutable_test() {
  if (!has_test()) {
    clear_data();
    set_has_test();
    data_.test_ = 
      ::google::protobuf::Arena::CreateMessage< ::msg::Action_Test >(
      GetArenaNoVirtual());
  }
  // @@protoc_insertion_point(field_mutable:msg.Action.test)
  return data_.test_;
}
::msg::Action_Test* Action::release_test() {
  // @@protoc_insertion_point(field_release:msg.Action.test)
  if (has_test()) {
    clear_has_data();
    if (GetArenaNoVirtual() != NULL) {
      ::msg::Action_Test* temp = new ::msg::Action_Test(*data_.test_);
      data_.test_ = NULL;
      return temp;
    } else {
      ::msg::Action_Test* temp = data_.test_;
      data_.test_ = NULL;
      return temp;
    }
  } else {
    return NULL;
  }
}
void Action::set_allocated_test(::msg::Action_Test* test) {
  clear_data();
  if (test) {
    if (GetArenaNoVirtual() != NULL &&
        ::google::protobuf::Arena::GetArena(test) == NULL) {
      GetArenaNoVirtual()->Own(test);
    } else if (GetArenaNoVirtual() !=
               ::google::protobuf::Arena::GetArena(test)) {
      ::msg::Action_Test* new_test = 
          ::google::protobuf::Arena::CreateMessage< ::msg::Action_Test >(
          GetArenaNoVirtual());
      new_test->CopyFrom(*test);
      test = new_test;
    }
    set_has_test();
    data_.test_ = test;
  }
  // @@protoc_insertion_point(field_set_allocated:msg.Action.test)
}
 ::msg::Action_Test* Action::unsafe_arena_release_test() {
  // @@protoc_insertion_point(field_unsafe_arena_release:msg.Action.test)
  if (has_test()) {
    clear_has_data();
    ::msg::Action_Test* temp = data_.test_;
    data_.test_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
 void Action::unsafe_arena_set_allocated_test(::msg::Action_Test* test) {
  clear_data();
  if (test) {
    set_has_test();
    data_.test_ = test;
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:msg.Action.test)
}

bool Action::has_data() const {
  return data_case() != DATA_NOT_SET;
}
void Action::clear_has_data() {
  _oneof_case_[0] = DATA_NOT_SET;
}
Action::DataCase Action::data_case() const {
  return Action::DataCase(_oneof_case_[0]);
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Actions::kActionFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Actions::Actions()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_actions_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:msg.Actions)
}
Actions::Actions(::google::protobuf::Arena* arena)
  : ::google::protobuf::Message(),
  _internal_metadata_(arena),
  action_(arena) {
  protobuf_actions_2eproto::InitDefaults();
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:msg.Actions)
}
Actions::Actions(const Actions& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      action_(from.action_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:msg.Actions)
}

void Actions::SharedCtor() {
  _cached_size_ = 0;
}

Actions::~Actions() {
  // @@protoc_insertion_point(destructor:msg.Actions)
  SharedDtor();
}

void Actions::SharedDtor() {
  ::google::protobuf::Arena* arena = GetArenaNoVirtual();
  GOOGLE_DCHECK(arena == NULL);
  if (arena != NULL) {
    return;
  }

}

void Actions::ArenaDtor(void* object) {
  Actions* _this = reinterpret_cast< Actions* >(object);
  (void)_this;
}
void Actions::RegisterArenaDtor(::google::protobuf::Arena* arena) {
}
void Actions::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Actions::descriptor() {
  protobuf_actions_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_actions_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Actions& Actions::default_instance() {
  protobuf_actions_2eproto::InitDefaults();
  return *internal_default_instance();
}

Actions* Actions::New(::google::protobuf::Arena* arena) const {
  return ::google::protobuf::Arena::CreateMessage<Actions>(arena);
}

void Actions::Clear() {
// @@protoc_insertion_point(message_clear_start:msg.Actions)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  action_.Clear();
  _internal_metadata_.Clear();
}

bool Actions::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:msg.Actions)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .msg.Action action = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_action()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:msg.Actions)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:msg.Actions)
  return false;
#undef DO_
}

void Actions::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:msg.Actions)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .msg.Action action = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->action_size()); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->action(static_cast<int>(i)), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:msg.Actions)
}

::google::protobuf::uint8* Actions::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:msg.Actions)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .msg.Action action = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->action_size()); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        1, this->action(static_cast<int>(i)), deterministic, target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:msg.Actions)
  return target;
}

size_t Actions::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:msg.Actions)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // repeated .msg.Action action = 1;
  {
    unsigned int count = static_cast<unsigned int>(this->action_size());
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->action(static_cast<int>(i)));
    }
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Actions::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:msg.Actions)
  GOOGLE_DCHECK_NE(&from, this);
  const Actions* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Actions>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:msg.Actions)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:msg.Actions)
    MergeFrom(*source);
  }
}

void Actions::MergeFrom(const Actions& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:msg.Actions)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  action_.MergeFrom(from.action_);
}

void Actions::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:msg.Actions)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Actions::CopyFrom(const Actions& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:msg.Actions)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Actions::IsInitialized() const {
  return true;
}

void Actions::Swap(Actions* other) {
  if (other == this) return;
  if (GetArenaNoVirtual() == other->GetArenaNoVirtual()) {
    InternalSwap(other);
  } else {
    Actions* temp = New(GetArenaNoVirtual());
    temp->MergeFrom(*other);
    other->CopyFrom(*this);
    InternalSwap(temp);
    if (GetArenaNoVirtual() == NULL) {
      delete temp;
    }
  }
}
void Actions::UnsafeArenaSwap(Actions* other) {
  if (other == this) return;
  GOOGLE_DCHECK(GetArenaNoVirtual() == other->GetArenaNoVirtual());
  InternalSwap(other);
}
void Actions::InternalSwap(Actions* other) {
  using std::swap;
  action_.InternalSwap(&other->action_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Actions::GetMetadata() const {
  protobuf_actions_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_actions_2eproto::file_level_metadata[kIndexInFileMessages];
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Actions

// repeated .msg.Action action = 1;
int Actions::action_size() const {
  return action_.size();
}
void Actions::clear_action() {
  action_.Clear();
}
const ::msg::Action& Actions::action(int index) const {
  // @@protoc_insertion_point(field_get:msg.Actions.action)
  return action_.Get(index);
}
::msg::Action* Actions::mutable_action(int index) {
  // @@protoc_insertion_point(field_mutable:msg.Actions.action)
  return action_.Mutable(index);
}
::msg::Action* Actions::add_action() {
  // @@protoc_insertion_point(field_add:msg.Actions.action)
  return action_.Add();
}
::google::protobuf::RepeatedPtrField< ::msg::Action >*
Actions::mutable_action() {
  // @@protoc_insertion_point(field_mutable_list:msg.Actions.action)
  return &action_;
}
const ::google::protobuf::RepeatedPtrField< ::msg::Action >&
Actions::action() const {
  // @@protoc_insertion_point(field_list:msg.Actions.action)
  return action_;
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace msg

// @@protoc_insertion_point(global_scope)
