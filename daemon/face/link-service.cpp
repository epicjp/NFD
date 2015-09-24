/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2015,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "link-service.hpp"
#include "lp-face.hpp"
#include "transport.hpp"

namespace nfd {
namespace face {

NFD_LOG_INIT("LinkService");

LinkService::LinkService()
  : m_face(nullptr)
  , m_transport(nullptr)
  , m_counters(nullptr)
{
}

LinkService::~LinkService()
{
}

void
LinkService::setFaceAndTransport(LpFace& face, Transport& transport)
{
  BOOST_ASSERT(m_face == nullptr);
  BOOST_ASSERT(m_transport == nullptr);

  m_face = &face;
  m_transport = &transport;
  m_counters = &m_face->getMutableCounters();
}

void
LinkService::sendInterest(const Interest& interest)
{
  BOOST_ASSERT(m_transport != nullptr);
  NFD_LOG_FACE_TRACE(__func__);

  ++m_counters->getNOutInterests();

  doSendInterest(interest);
}

void
LinkService::sendData(const Data& data)
{
  BOOST_ASSERT(m_transport != nullptr);
  NFD_LOG_FACE_TRACE(__func__);

  ++m_counters->getNOutDatas();

  doSendData(data);
}

void
LinkService::sendNack(const ndn::lp::Nack& nack)
{
  BOOST_ASSERT(m_transport != nullptr);
  NFD_LOG_FACE_TRACE(__func__);

  // TODO#3177 increment counter

  doSendNack(nack);
}

void
LinkService::receiveInterest(const Interest& interest)
{
  NFD_LOG_FACE_TRACE(__func__);

  ++m_counters->getNInInterests();

  afterReceiveInterest(interest);
}

void
LinkService::receiveData(const Data& data)
{
  NFD_LOG_FACE_TRACE(__func__);

  ++m_counters->getNInDatas();

  afterReceiveData(data);
}

void
LinkService::receiveNack(const ndn::lp::Nack& nack)
{
  NFD_LOG_FACE_TRACE(__func__);

  // TODO#3177 increment counter

  afterReceiveNack(nack);
}

std::ostream&
operator<<(std::ostream& os, const FaceLogHelper<LinkService>& flh)
{
  const LpFace* face = flh.obj.getFace();
  if (face == nullptr) {
    os << "[id=0,local=unknown,remote=unknown] ";
  }
  else {
    os << "[id=" << face->getId() << ",local=" << face->getLocalUri()
       << ",remote=" << face->getRemoteUri() << "] ";
  }
  return os;
}

} // namespace face
} // namespace nfd