<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title>JSTL NetCalc</title>
	</head>
	<c:set var="ip" value="${ param.ip }" />
	<c:set var="netmask" value="${ param.n }" />
	<jsp:useBean id="calc_bean" class="cloud.macca.NetCalcBean" /> 
	<body>
		<h1> NetCalc w/ JSTL </h1>
		<form method="POST" action="/Calc.jsp">
			<span style="display: block; margin-bottom: 10px">IP: <input type="text" name="ip" value="<c:out value="${ ip }" />" /></span>
			<span style="display: block; margin-bottom: 10px;"> Netmask (CIDR): <input type="number" name="n" value="<c:out value="${ netmask }" />" /></span>
			<button> Esegui calcolo </button>
		</form>
		<c:if test="${ ip != null && netmask != null }">
			<jsp:setProperty property="IP" name="calc_bean" value="${ ip }"/>
			<jsp:setProperty property="netmask" name="calc_bean" value="${ netmask }"/>
			<c:catch var="exception">
				<ul>
					<li> IP: <c:out value="${ calc_bean.stringifyIp(calc_bean.intIp) }" /> </li>
					<li> NetID: <c:out value="${ calc_bean.stringifyIp(calc_bean.netId) }" /> </li>
					<li> Netmask: <c:out value="${ calc_bean.stringifyIp(calc_bean.dottedNetmask) }" /> </li>
				</ul>
			</c:catch>
			<c:if test="${ exception != null }">
				<c:out value="L'IP inserito non è valido" />
			</c:if>
		</c:if>
	</body>
</html>