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

#include <2lgc/config.h>

#ifdef OPENSSL_FOUND

#include <2lgc/error/show.h>
#include <2lgc/net/openssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <mutex>

/** \class llgc::net::OpenSsl
 * \brief Class to manipulate OpenSSL functions. All functions are thread-safe.
 */

/// \brief Run init global function.
void llgc::net::OpenSsl::Init()
{
  std::lock_guard guardian(mutex_);
  if (!init_done)
  {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    init_done = true;
  }
}

/// \brief Run initial load of error strings for OpenSSL.
void llgc::net::OpenSsl::InitErr()
{
  std::lock_guard guardian(mutex_);
  if (!init_err_done)
  {
    SSL_load_error_strings();
    init_err_done = true;
  }
}

/** @brief Create a context for client based on presentation.
 * @param[out] presentation The encryption wanted.
 * @param[in] ctx The new context.
 * @param[in] ssl The SSL connexion state.
 * @return true if no problem.
 */
bool llgc::net::OpenSsl::InitCtxClient(Presentation presentation,
                                       std::shared_ptr<SSL_CTX> *ctx,
                                       std::shared_ptr<SSL> *ssl,
                                       int client_sock)
{
  BUGLIB(std::cout,
         *ctx = InitCtx(true, presentation),
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");

  return true;
}

/** @brief Create a SSL connexion state for client based on presentation.
 * @param[out] presentation The encryption wanted.
 * @param[in] ctx The new context.
 * @param[in] ssl The SSL connexion state.
 * @return true if no problem.
 */
bool llgc::net::OpenSsl::InitSslClient(Presentation presentation,
                                       std::shared_ptr<SSL_CTX> *ctx,
                                       std::shared_ptr<SSL> *ssl,
                                       int client_sock)
{
  BUGLIB(std::cout,
         *ssl = InitSsl(*ctx),
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");

  BUGLIB(std::cout, SSL_set_fd(ssl->get(), client_sock) == 1,
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");

  BUGCRIT(std::cout, SSL_connect(ssl->get()) == 1,
          (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
          "Failed to initialize connection.\n");

  return true;
}

/** @brief Create a context and a SSL connexion state for server based on
 *         presentation.
 *         Must be call after accept4 function.
 * @param[in] presentation The encryption wanted.
 * @param[out] ctx The new context.
 * @param[out] ssl The SSL connexion state.
 * @return true if no problem.
 */
bool llgc::net::OpenSsl::InitCtxSslServer(Presentation presentation,
                                          std::shared_ptr<SSL_CTX> *ctx,
                                          std::shared_ptr<SSL> *ssl,
                                          const std::string& cert,
                                          const std::string& key,
                                          int client_sock)
{
  BUGLIB(std::cout,
         *ctx = InitCtx(false, presentation),
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");

  // Certificate and key must be load before calling SSL_new.
  BUGLIB(std::cout,
         SSL_CTX_use_certificate_file(ctx->get(), cert.c_str(),
                                      SSL_FILETYPE_PEM) == 1,
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");
  BUGLIB(std::cout,
         SSL_CTX_use_PrivateKey_file(ctx->get(), key.c_str(),
                                     SSL_FILETYPE_PEM) == 1,
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");
  BUGUSER(std::cout, SSL_CTX_check_private_key(ctx->get()) == 1,
          (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
          "Private key does not match the public certificate.\n");

  BUGLIB(std::cout,
         *ssl = InitSsl(*ctx),
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");

  BUGLIB(std::cout, SSL_set_fd(ssl->get(), client_sock) == 1,
         (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
         "OpenSSL");
  BUGCRIT(std::cout, SSL_accept(ssl->get()) == 1,
          (llgc::net::OpenSsl::InitErr(), ERR_print_errors_fp(stdout), false),
          "Failed to initialize handshake.\n");

  return true;
}

/** @brief Create a context based on presentation.
 * @param[in] client true if client is asked, false if server.
 * @param[in] presentation The encryption wanted.
 * @return The new context if no problem.
 */
std::shared_ptr<SSL_CTX> llgc::net::OpenSsl::InitCtx(bool client,
                                                     Presentation presentation)
{
  Init();

  const SSL_METHOD *method;
  switch (presentation)
  {
    case llgc::net::OpenSsl::Presentation::TSL1_2:
    {
      if (client)
      {
        method = SSLv23_client_method();
      }
      else
      {
        method = SSLv23_server_method();
      }
      break;
    }
    case llgc::net::OpenSsl::Presentation::NONE:
    default:
    {
      BUGPROG(std::cout, false, nullptr, "Invalid value in switch.");
    }
  }

  // Only one context for all connections
  return std::shared_ptr<SSL_CTX>(SSL_CTX_new(method),
                                  [](SSL_CTX *ptr) { SSL_CTX_free(ptr); });
}

/** @brief Create a SSL connexion state based on presentation.
 * @param[in] client true if client is asked, false if server.
 * @param[in] presentation The encryption wanted.
 * @param[out] ctx The new context.
 * @param[out] ssl The SSL connexion state.
 * @return true if no problem.
 */
std::shared_ptr<SSL> llgc::net::OpenSsl::InitSsl(std::shared_ptr<SSL_CTX> ctx)
{
  return std::shared_ptr<SSL>(SSL_new(ctx.get()),
                              [](SSL *ptr) { SSL_free(ptr); });
}

/// \brief Every function is thread-safe so we need a lock.
std::mutex llgc::net::OpenSsl::mutex_;

/// \brief true if ssl library is init.
bool llgc::net::OpenSsl::init_done = false;

/// \brief true if ssl strings are loaded.
bool llgc::net::OpenSsl::init_err_done = false;
#endif  // OPENSSL_FOUND

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
