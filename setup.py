import setuptools

setuptools.setup(
    name="fast-knn",
    version="0.0.1",
    author="Jonas",
    author_email="jonas@valfridsson.net",
    description="A fast KMeans and KNN implementation",
    url="",
    packages=setuptools.find_packages(),
    install_requires=["numpy"],
    package_data={'': ['fast_knn.so']},
    include_package_data=True,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)
