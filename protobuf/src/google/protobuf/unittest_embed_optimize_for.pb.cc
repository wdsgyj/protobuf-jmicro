// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "google/protobuf/unittest_embed_optimize_for.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace protobuf_unittest {

namespace {

const ::google::protobuf::Descriptor* TestEmbedOptimizedForSize_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  TestEmbedOptimizedForSize_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto() {
  protobuf_AddDesc_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "google/protobuf/unittest_embed_optimize_for.proto");
  GOOGLE_CHECK(file != NULL);
  TestEmbedOptimizedForSize_descriptor_ = file->message_type(0);
  static const int TestEmbedOptimizedForSize_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestEmbedOptimizedForSize, optional_message_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestEmbedOptimizedForSize, repeated_message_),
  };
  TestEmbedOptimizedForSize_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      TestEmbedOptimizedForSize_descriptor_,
      TestEmbedOptimizedForSize::default_instance_,
      TestEmbedOptimizedForSize_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestEmbedOptimizedForSize, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(TestEmbedOptimizedForSize, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(TestEmbedOptimizedForSize));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    TestEmbedOptimizedForSize_descriptor_, &TestEmbedOptimizedForSize::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto() {
  delete TestEmbedOptimizedForSize::default_instance_;
  delete TestEmbedOptimizedForSize_reflection_;
}

void protobuf_AddDesc_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::protobuf_unittest::protobuf_AddDesc_google_2fprotobuf_2funittest_5foptimize_5ffor_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n1google/protobuf/unittest_embed_optimiz"
    "e_for.proto\022\021protobuf_unittest\032+google/p"
    "rotobuf/unittest_optimize_for.proto\"\241\001\n\031"
    "TestEmbedOptimizedForSize\022A\n\020optional_me"
    "ssage\030\001 \001(\0132\'.protobuf_unittest.TestOpti"
    "mizedForSize\022A\n\020repeated_message\030\002 \003(\0132\'"
    ".protobuf_unittest.TestOptimizedForSizeB"
    "\002H\001", 283);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "google/protobuf/unittest_embed_optimize_for.proto", &protobuf_RegisterTypes);
  TestEmbedOptimizedForSize::default_instance_ = new TestEmbedOptimizedForSize();
  TestEmbedOptimizedForSize::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto {
  StaticDescriptorInitializer_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto() {
    protobuf_AddDesc_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto();
  }
} static_descriptor_initializer_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int TestEmbedOptimizedForSize::kOptionalMessageFieldNumber;
const int TestEmbedOptimizedForSize::kRepeatedMessageFieldNumber;
#endif  // !_MSC_VER

TestEmbedOptimizedForSize::TestEmbedOptimizedForSize()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void TestEmbedOptimizedForSize::InitAsDefaultInstance() {
  optional_message_ = const_cast< ::protobuf_unittest::TestOptimizedForSize*>(&::protobuf_unittest::TestOptimizedForSize::default_instance());
}

TestEmbedOptimizedForSize::TestEmbedOptimizedForSize(const TestEmbedOptimizedForSize& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void TestEmbedOptimizedForSize::SharedCtor() {
  _cached_size_ = 0;
  optional_message_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

TestEmbedOptimizedForSize::~TestEmbedOptimizedForSize() {
  SharedDtor();
}

void TestEmbedOptimizedForSize::SharedDtor() {
  if (this != default_instance_) {
    delete optional_message_;
  }
}

void TestEmbedOptimizedForSize::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* TestEmbedOptimizedForSize::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return TestEmbedOptimizedForSize_descriptor_;
}

const TestEmbedOptimizedForSize& TestEmbedOptimizedForSize::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_google_2fprotobuf_2funittest_5fembed_5foptimize_5ffor_2eproto();  return *default_instance_;
}

TestEmbedOptimizedForSize* TestEmbedOptimizedForSize::default_instance_ = NULL;

TestEmbedOptimizedForSize* TestEmbedOptimizedForSize::New() const {
  return new TestEmbedOptimizedForSize;
}

void TestEmbedOptimizedForSize::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (optional_message_ != NULL) optional_message_->::protobuf_unittest::TestOptimizedForSize::Clear();
    }
  }
  repeated_message_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool TestEmbedOptimizedForSize::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .protobuf_unittest.TestOptimizedForSize optional_message = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_optional_message()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_repeated_message;
        break;
      }
      
      // repeated .protobuf_unittest.TestOptimizedForSize repeated_message = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_repeated_message:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_repeated_message()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_repeated_message;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void TestEmbedOptimizedForSize::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional .protobuf_unittest.TestOptimizedForSize optional_message = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->optional_message(), output);
  }
  
  // repeated .protobuf_unittest.TestOptimizedForSize repeated_message = 2;
  for (int i = 0; i < this->repeated_message_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      2, this->repeated_message(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* TestEmbedOptimizedForSize::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional .protobuf_unittest.TestOptimizedForSize optional_message = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->optional_message(), target);
  }
  
  // repeated .protobuf_unittest.TestOptimizedForSize repeated_message = 2;
  for (int i = 0; i < this->repeated_message_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->repeated_message(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int TestEmbedOptimizedForSize::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .protobuf_unittest.TestOptimizedForSize optional_message = 1;
    if (has_optional_message()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->optional_message());
    }
    
  }
  // repeated .protobuf_unittest.TestOptimizedForSize repeated_message = 2;
  total_size += 1 * this->repeated_message_size();
  for (int i = 0; i < this->repeated_message_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->repeated_message(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void TestEmbedOptimizedForSize::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const TestEmbedOptimizedForSize* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const TestEmbedOptimizedForSize*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void TestEmbedOptimizedForSize::MergeFrom(const TestEmbedOptimizedForSize& from) {
  GOOGLE_CHECK_NE(&from, this);
  repeated_message_.MergeFrom(from.repeated_message_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_optional_message()->::protobuf_unittest::TestOptimizedForSize::MergeFrom(from.optional_message());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void TestEmbedOptimizedForSize::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void TestEmbedOptimizedForSize::CopyFrom(const TestEmbedOptimizedForSize& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TestEmbedOptimizedForSize::IsInitialized() const {
  
  if (has_optional_message()) {
    if (!this->optional_message().IsInitialized()) return false;
  }
  for (int i = 0; i < repeated_message_size(); i++) {
    if (!this->repeated_message(i).IsInitialized()) return false;
  }
  return true;
}

void TestEmbedOptimizedForSize::Swap(TestEmbedOptimizedForSize* other) {
  if (other != this) {
    std::swap(optional_message_, other->optional_message_);
    repeated_message_.Swap(&other->repeated_message_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata TestEmbedOptimizedForSize::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = TestEmbedOptimizedForSize_descriptor_;
  metadata.reflection = TestEmbedOptimizedForSize_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf_unittest

// @@protoc_insertion_point(global_scope)
