/*
 *    Copyright 2018 LE GARREC Vincent
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <2lgc/error/show.h>
#include <2lgc/pattern/strategy.h>
#include <google/protobuf/stubs/common.h>

/**
 * @brief Interface of a command.
 */
class StrategyAddOne : public llgc::pattern::Strategy<int>
{
 public:
  explicit StrategyAddOne(int *number) : llgc::pattern::Strategy<int>(number) {}

  bool Do() override CHK
  {
    (*instance_)++;
    return true;
  }
};

class StrategyAddNumber : public llgc::pattern::Strategy<int>
{
 public:
  StrategyAddNumber(int *number, int add)
      : llgc::pattern::Strategy<int>(number), add_(add)
  {
  }

  bool Do() override CHK
  {
    (*instance_) += add_;
    return true;
  }

 private:
  const int add_;
};

#ifdef TEMPLATE_CLASS
#include <2lgc/config.h>

#include <2lgc/pattern/strategy.cc>

template class llgc::pattern::Strategy<int>;
#endif

int main(int /* argc */, char * /* argv */ [])  // NS
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  int number = 0;

  StrategyAddOne strategy1(&number);
  StrategyAddNumber strategy2(&number, 1);

  assert(number == 0);
  EXECUTE_AND_ABORT(std::cout, strategy1.Do());
  assert(number == 1);
  EXECUTE_AND_ABORT(std::cout, strategy2.Do());
  assert(number == 2);

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
