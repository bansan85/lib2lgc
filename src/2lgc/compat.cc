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

/**
 * \file compat.h
 * \brief Various define for compatibility between Visual Studio and GCC.
 *
 *
 * \def CHK
 * \brief Compatible `__attribute__((__warn_unused_result__))` for VS and GCC.
 *
 *
 * \def SSCANF
 * \brief Compatible `sscanf` for VS and GCC.
 *
 *
 * \def DllExport
 * \brief Compatible `dllexport` for VS and GCC.
 *
 *
 * \def INLINE_TEMPLATE
 * \brief inline template functions if `template class` is not enabled.
 */

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
