<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <!-- Parameters -->
  <xsl:param name="enableTypes" select="0" />
  <xsl:param name="typeAttributeName" select="'type'" />
  <xsl:param name="attrStyle" select="'verbose'" />
  <xsl:param name="rootElementName" select="'expr'" />
  <xsl:param name="listElementName" select="'elem'" />
  <xsl:param name="attrElementName" select="'attr'" />
  <xsl:param name="nameAttributeName" select="'name'" />

  <!-- Transformation templates -->
  <xsl:template match="string | path | int | float | bool">
    <xsl:choose>
      <xsl:when test="$enableTypes">
        <xsl:attribute name="{$typeAttributeName}">
          <xsl:value-of select="local-name()" />
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>

    <xsl:value-of select="@value" />
  </xsl:template>

  <xsl:template match="list">
    <xsl:choose>
      <xsl:when test="$enableTypes">
        <xsl:attribute name="{$typeAttributeName}">list</xsl:attribute>
      </xsl:when>
    </xsl:choose>

    <xsl:for-each select="*">
      <xsl:element name="{$listElementName}">
        <xsl:apply-templates select="." />
      </xsl:element>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="convert_attrs_simple">
    <xsl:for-each select="attr">
      <xsl:element name="{@name}">
        <xsl:apply-templates select="*" />
      </xsl:element>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="convert_attrs_verbose">
    <xsl:for-each select="attr">
      <xsl:element name="{$attrElementName}">
        <xsl:attribute name="{$nameAttributeName}">
          <xsl:value-of select="@name" />
        </xsl:attribute>

        <xsl:apply-templates select="*" />
      </xsl:element>
    </xsl:for-each>
  </xsl:template>

  <xsl:template match="attrs">
    <xsl:choose>
      <xsl:when test="$enableTypes">
        <xsl:attribute name="{$typeAttributeName}">attrs</xsl:attribute>
      </xsl:when>
    </xsl:choose>

    <xsl:choose>
      <xsl:when test="$attrStyle='simple'">
        <xsl:call-template name="convert_attrs_simple" />
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="convert_attrs_verbose" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Root node transformation -->
  <xsl:template match="/expr">
    <xsl:element name="{$rootElementName}">
      <xsl:apply-templates select="*" />
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>
