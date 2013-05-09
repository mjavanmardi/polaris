@echo off

echo Copying the database
cp -rf z:\POLARIS\interchange\with_its\chicago-Supply.sqlite .
echo Running spatialite
spatialite chicago-Supply.sqlite < subarea_spatial.sql

echo Running sqlite3
sqlite3 chicago-Supply.sqlite < subarea.sql

REM cp -rf chicago-Supply.sqlite z:\POLARIS\interchange\Subarea
REM cp link.* z:\POLARIS\interchange\Subarea
REM cp node.* z:\POLARIS\interchange\Subarea
REM cp zone.* z:\POLARIS\interchange\Subarea