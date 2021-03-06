// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "BenchmarkClient.h"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <sstream>
#include <string_view>

using namespace std::literals;

namespace graphql::client {

using namespace query::Query;

template <>
Response::appointments_AppointmentConnection::pageInfo_PageInfo ModifiedResponse<Response::appointments_AppointmentConnection::pageInfo_PageInfo>::parse(response::Value&& response)
{
	Response::appointments_AppointmentConnection::pageInfo_PageInfo result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(hasNextPage)js"sv)
			{
				result.hasNextPage = ModifiedResponse<response::BooleanType>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment ModifiedResponse<Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment>::parse(response::Value&& response)
{
	Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(id)js"sv)
			{
				result.id = ModifiedResponse<response::IdType>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(when)js"sv)
			{
				result.when = ModifiedResponse<response::Value>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(subject)js"sv)
			{
				result.subject = ModifiedResponse<response::StringType>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
			if (member.first == R"js(isNow)js"sv)
			{
				result.isNow = ModifiedResponse<response::BooleanType>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
Response::appointments_AppointmentConnection::edges_AppointmentEdge ModifiedResponse<Response::appointments_AppointmentConnection::edges_AppointmentEdge>::parse(response::Value&& response)
{
	Response::appointments_AppointmentConnection::edges_AppointmentEdge result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(node)js"sv)
			{
				result.node = ModifiedResponse<Response::appointments_AppointmentConnection::edges_AppointmentEdge::node_Appointment>::parse<TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

template <>
Response::appointments_AppointmentConnection ModifiedResponse<Response::appointments_AppointmentConnection>::parse(response::Value&& response)
{
	Response::appointments_AppointmentConnection result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(pageInfo)js"sv)
			{
				result.pageInfo = ModifiedResponse<Response::appointments_AppointmentConnection::pageInfo_PageInfo>::parse(std::move(member.second));
				continue;
			}
			if (member.first == R"js(edges)js"sv)
			{
				result.edges = ModifiedResponse<Response::appointments_AppointmentConnection::edges_AppointmentEdge>::parse<TypeModifier::Nullable, TypeModifier::List, TypeModifier::Nullable>(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

namespace query::Query {

const std::string& GetRequestText() noexcept
{
	static const auto s_request = R"gql(
		# Copyright (c) Microsoft Corporation. All rights reserved.
		# Licensed under the MIT License.
		
		query {
		  appointments {
		    pageInfo {
		      hasNextPage
		    }
		    edges {
		      node {
		        id
		        when
		        subject
		        isNow
		      }
		    }
		  }
		}
	)gql"s;

	return s_request;
}

const peg::ast& GetRequestObject() noexcept
{
	static const auto s_request = []() noexcept {
		auto ast = peg::parseString(GetRequestText());

		// This has already been validated against the schema by clientgen.
		ast.validated = true;

		return ast;
	}();

	return s_request;
}

Response parseResponse(response::Value&& response)
{
	Response result;

	if (response.type() == response::Type::Map)
	{
		auto members = response.release<response::MapType>();

		for (auto& member : members)
		{
			if (member.first == R"js(appointments)js"sv)
			{
				result.appointments = ModifiedResponse<Response::appointments_AppointmentConnection>::parse(std::move(member.second));
				continue;
			}
		}
	}

	return result;
}

} // namespace query::Query
} // namespace graphql::client
