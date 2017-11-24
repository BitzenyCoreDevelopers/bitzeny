Release Process
====================

* update translations (ping wumpus, Diapolo or tcatm on IRC)
* see https://github.com/BitzenyCoreDevelopers/bitzeny/blob/z1.2.x/doc/translation_process.md#syncing-with-transifex

* * *

###update (commit) version in sources

	contrib/verifysfbinaries/verify.sh
	doc/README*
	share/setup.nsi
	src/clientversion.h (change CLIENT_VERSION_IS_RELEASE to true)

###tag version in git

	git tag -s z(new version, e.g. 1.2.0)

###write release notes. git shortlog helps a lot, for example:

	git shortlog --no-merges z(current version, e.g. 1.1.0)..z(new version, e.g. 1.2.0)

* * *

##perform gitian builds

 From a directory containing the bitzeny source, gitian-builder and gitian.sigs

	export SIGNER=(your github username)
	export VERSION=(new version, e.g. 1.2.0)
	pushd ./bitzeny
	git checkout z${VERSION}
	popd
	pushd ./gitian-builder
	mkdir -p inputs; cd inputs/

 Register and download the Apple SDK (see OSX Readme for details)
	visit https://download.developer.apple.com/Developer_Tools/xcode_4.6.3/xcode4630916281a.dmg
 
 Using a Mac, create a tarball for the 10.7 SDK
	tar -C /Volumes/Xcode/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/ -czf MacOSX10.7.sdk.tar.gz MacOSX10.7.sdk

 Alternatively, you can use 7zip and SleuthKit to extract the files one by one.
 The script contrib/macdeploy/extract-osx-sdk.sh automates this. First ensure
 the dmg file is in the current directory, and then run the script. You may wish
 to delete the intermediate 3.hfs file and MacOSX10.7.sdk (the directory) when
 you've confirmed the extraction succeeded.

	apt-get install p7zip-full sleuthkit
	contrib/macdeploy/extract-osx-sdk.sh
	rm -rf 3.hfs MacOSX10.7.sdk

 Fetch and build inputs: (first time, or when dependency versions change)

	wget 'http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.9.20150206.tar.gz' -O miniupnpc-1.9.20150206.tar.gz
	wget 'https://www.openssl.org/source/openssl-1.0.2l.tar.gz'
	wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
	wget 'http://zlib.net/zlib-1.2.11.tar.gz'
	wget 'ftp://ftp.simplesystems.org/pub/png/src/history/libpng16/libpng-1.6.8.tar.gz'
	wget 'https://fukuchi.org/works/qrencode/qrencode-3.4.3.tar.bz2'
	wget 'https://downloads.sourceforge.net/project/boost/boost/1.55.0/boost_1_55_0.tar.bz2'
	wget 'https://svn.boost.org/trac/boost/raw-attachment/ticket/7262/boost-mingw.patch' -O \ 
	     boost-mingw-gas-cross-compile-2013-03-03.patch
	wget 'https://download.qt.io/archive/qt/5.2/5.2.0/single/qt-everywhere-opensource-src-5.2.0.tar.gz'
	wget 'https://download.qt-project.org/archive/qt/4.6/qt-everywhere-opensource-src-4.6.4.tar.gz'
	wget 'https://protobuf.googlecode.com/files/protobuf-2.5.0.tar.bz2'
	wget 'https://github.com/mingwandroid/toolchain4/archive/10cc648683617cca8bcbeae507888099b41b530c.tar.gz'
	wget 'http://www.opensource.apple.com/tarballs/cctools/cctools-809.tar.gz'
	wget 'http://www.opensource.apple.com/tarballs/dyld/dyld-195.5.tar.gz'
	wget 'http://www.opensource.apple.com/tarballs/ld64/ld64-127.2.tar.gz'
	wget 'http://pkgs.fedoraproject.org/repo/pkgs/cdrkit/cdrkit-1.1.11.tar.gz/efe08e2f3ca478486037b053acd512e9/cdrkit-1.1.11.tar.gz'
	wget 'https://github.com/theuni/libdmg-hfsplus/archive/libdmg-hfsplus-v0.1.tar.gz'
	wget 'http://llvm.org/releases/3.2/clang+llvm-3.2-x86-linux-ubuntu-12.04.tar.gz' -O \
	     clang-llvm-3.2-x86-linux-ubuntu-12.04.tar.gz
	wget 'https://raw.githubusercontent.com/theuni/osx-cross-depends/master/patches/cdrtools/genisoimage.diff' -O \
	     cdrkit-deterministic.patch
	cd ..
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/boost-linux.yml
	mv build/out/boost-*.zip inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/deps-linux.yml
	mv build/out/bitcoin-deps-*.zip inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/qt-linux.yml
	mv build/out/qt-*.tar.gz inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/boost-win.yml
	mv build/out/boost-*.zip inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/deps-win.yml
	mv build/out/bitcoin-deps-*.zip inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/qt-win.yml
	mv build/out/qt-*.zip inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/protobuf-win.yml
	mv build/out/protobuf-*.zip inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/gitian-osx-native.yml
	mv build/out/osx-*.tar.gz inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/gitian-osx-depends.yml
	mv build/out/osx-*.tar.gz inputs/
	./bin/gbuild ../bitzeny/contrib/gitian-descriptors/gitian-osx-qt.yml
	mv build/out/osx-*.tar.gz inputs/

 The expected SHA256 hashes of the intermediate inputs are:

	63e4f7f9ff16cc8832c851bfe132127735e47cd157de572f10af00254d3bdae3  bitcoin-deps-linux32-gitian-r9.zip
	f7eab18d45887161532288517789bbf2b75cc12bf70e896c8f86f1d9f1997feb  bitcoin-deps-linux64-gitian-r9.zip
	f29b7d9577417333fb56e023c2977f5726a7c297f320b175a4108cf7cd4c2d29  boost-linux32-1.55.0-gitian-r1.zip
	88232451c4104f7eb16e469ac6474fd1231bd485687253f7b2bdf46c0781d535  boost-linux64-1.55.0-gitian-r1.zip
	57e57dbdadc818cd270e7e00500a5e1085b3bcbdef69a885f0fb7573a8d987e1  qt-linux32-4.6.4-gitian-r1.tar.gz
	60eb4b9c5779580b7d66529efa5b2836ba1a70edde2a0f3f696d647906a826be  qt-linux64-4.6.4-gitian-r1.tar.gz
	60dc2d3b61e9c7d5dbe2f90d5955772ad748a47918ff2d8b74e8db9b1b91c909  boost-win32-1.55.0-gitian-r6.zip
	f65fcaf346bc7b73bc8db3a8614f4f6bee2f61fcbe495e9881133a7c2612a167  boost-win64-1.55.0-gitian-r6.zip
	576ca55db9b710b6a2e6802fa88046288bcb3d58b422a602edd790d36745e0ff  bitcoin-deps-win32-gitian-r16.zip
	9f6381f56ecb9467dfce4cd78018780c3f703fc091fe6cec52342d783d2576ae  bitcoin-deps-win64-gitian-r16.zip
	eab65e30aa6e3c9efac8613e6337093d718bf037fed136dabcbf1e0f789bcea4  qt-win32-5.2.0-gitian-r3.zip
	2cfae7afcc3db774360cab50124c8124d0f6ca6ee1ee35b8fd00d9d0a4751f19  qt-win64-5.2.0-gitian-r3.zip
	e2e403e1a08869c7eed4d4293bce13d51ec6a63592918b90ae215a0eceb44cb4  protobuf-win32-2.5.0-gitian-r4.zip
	a0999037e8b0ef9ade13efd88fee261ba401f5ca910068b7e0cd3262ba667db0  protobuf-win64-2.5.0-gitian-r4.zip

 Build bitzenyd and bitzeny-qt on Linux32, Linux64, and Win32:

	./bin/gbuild --commit bitzeny=z${VERSION} ../bitzeny/contrib/gitian-descriptors/gitian-linux.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-linux --destination ../gitian.sigs/ ../bitzeny/contrib/gitian-descriptors/gitian-linux.yml
	pushd build/out
	zip -r bitzeny-${VERSION}-linux-gitian.zip *
	mv bitzeny-${VERSION}-linux-gitian.zip ../../../
	popd
	./bin/gbuild --commit bitzeny=z${VERSION} ../bitzeny/contrib/gitian-descriptors/gitian-win.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-win --destination ../gitian.sigs/ ../bitzeny/contrib/gitian-descriptors/gitian-win.yml
	pushd build/out
	zip -r bitzeny-${VERSION}-win-gitian.zip *
	mv bitzeny-${VERSION}-win-gitian.zip ../../../
	popd
	./bin/gbuild --commit bitzeny=z${VERSION} ../bitzeny/contrib/gitian-descriptors/gitian-osx-bitcoin.yml
	./bin/gsign --signer $SIGNER --release ${VERSION}-osx --destination ../gitian.sigs/ ../bitzeny/contrib/gitian-descriptors/gitian-osx-bitcoin.yml
	pushd build/out
	mv BitZeny-Qt.dmg ../../../
	popd
	popd

  Build output expected:

  1. linux 32-bit and 64-bit binaries + source (bitzeny-${VERSION}-linux-gitian.zip)
  2. windows 32-bit and 64-bit binaries + installer + source (bitzeny-${VERSION}-win-gitian.zip)
  3. OSX installer (BitZeny-Qt.dmg)
  4. Gitian signatures (in gitian.sigs/${VERSION}[-win|-osx]/(your gitian key)/

repackage gitian builds for release as stand-alone zip/tar/installer exe

**Linux .tar.gz:**

	unzip bitzeny-${VERSION}-linux-gitian.zip -d bitzeny-${VERSION}-linux
	tar czvf bitzeny-${VERSION}-linux.tar.gz bitzeny-${VERSION}-linux
	rm -rf bitzeny-${VERSION}-linux

**Windows .zip and setup.exe:**

	unzip bitzeny-${VERSION}-win-gitian.zip -d bitzeny-${VERSION}-win
	mv bitzeny-${VERSION}-win/*/bitzeny-*-setup.exe .
	zip -r bitzeny-${VERSION}-win.zip bitzeny-${VERSION}-win
	rm -rf bitzeny-${VERSION}-win

**Mac OS X .dmg:**

	mv BitZeny-Qt.dmg bitzeny-${VERSION}-osx.dmg

###Next steps:

Commit your signature to gitian.sigs:

	pushd gitian.sigs
	git add ${VERSION}-linux/${SIGNER}
	git add ${VERSION}-win/${SIGNER}
	git add ${VERSION}-osx/${SIGNER}
	git commit -a
	git push  # Assuming you can push to the gitian.sigs tree
	popd

-------------------------------------------------------------------------

### After 3 or more people have gitian-built and their results match:

- Perform code-signing.

    - Code-sign Windows -setup.exe (in a Windows virtual machine using signtool)

    - Code-sign MacOSX .dmg

  Note: only ZNY.Dev has the code-signing keys currently.

- Create `SHA256SUMS.asc` for builds, and PGP-sign it. This is done manually.
  Include all the files to be uploaded.

SHA256SUMS
```
88909c816f4fc9f5fc0d10b6706db0fa2a7e40029b587dea256324d09dfdf8d2  bitzeny-1.2.0-linux.tar.gz
471d63dbedee9888a72969f032664b5f520a0f304f11e6e1bd9557463a0be88f  bitzeny-1.2.0-osx.dmg
6305c3bf611c03fcb3d8dd75912ff18c95edeedb403a856f838357d0d2a9c249  bitzeny-1.2.0-win.zip
19c561de87d1d4d554358d42ff5e0287a42b3396e4634336ba7f75470e1b9e44  bitzeny-1.2.0-win32-setup.exe
61c60c186944c0a56830d426a984deecca6d78798655099419ba4a14837018ea  bitzeny-1.2.0-win64-setup.exe
```

```
gpg --clearsign --digest-algo SHA256 SHA256SUMS
```

SHA256SUMS.asc
```
-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

88909c816f4fc9f5fc0d10b6706db0fa2a7e40029b587dea256324d09dfdf8d2  bitzeny-1.2.0-linux.tar.gz
471d63dbedee9888a72969f032664b5f520a0f304f11e6e1bd9557463a0be88f  bitzeny-1.2.0-osx.dmg
6305c3bf611c03fcb3d8dd75912ff18c95edeedb403a856f838357d0d2a9c249  bitzeny-1.2.0-win.zip
19c561de87d1d4d554358d42ff5e0287a42b3396e4634336ba7f75470e1b9e44  bitzeny-1.2.0-win32-setup.exe
61c60c186944c0a56830d426a984deecca6d78798655099419ba4a14837018ea  bitzeny-1.2.0-win64-setup.exe
-----BEGIN PGP SIGNATURE-----
...
```

- Upload zips and installers, as well as `SHA256SUMS.asc` from last step, to the https://github.com/BitzenyCoreDevelopers/bitzeny/releases server
