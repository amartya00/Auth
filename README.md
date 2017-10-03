## Auth - A fairly general and simple auth library for web services:

### What does it do?
  - Provide registration facilities for users.
  - Provides faciloties for whitelisting users of whitelist enabled services.

### General flow:
#### Registration / signin:
  - The library uses oauth tokens to authenticate users. Currently only google is supported.
  - Github and facebook is in pipeline.
  - The main input to the signin framework is the oauth token and the oauth provider.
  - The entire flow goes like this:
    - signin: call oauth provider to get user info
    - is whitelisting enabled for service?
    - if yes is the user whitelisted?
    - if so (or if whitelisting is not enabled) does the user info exist in our records?
    - if no, create a record (Schema detailed later).
    - return the user entry.

#### Authorization:
    - The library uses the uid and password generated at registration for authorizing a user.
    - The user info entry has a list of services each user is allowed to access.
    - A service is added to this list when the user registers to that service.
    - Operation level granularity for authorization is in the pipeline.
    - The entire flow goes like this:
      - authorize: is whitelisting enabled for service?
      - if yes, is user whitelisted?
      - if yes (or whitelisting is not enabled) does user entry exist?
      - if yes, is the service in the list of authorized services in users list?
      - if yes return true, else return false.

### Using the library:
#### AWS resources needed:
    - The library needs 3 tables:
      - Table containing list of services and the whitelisting status.
        - Schema:
        - Hash key: Service name
        - Other attributes: whitelisting status
      - Table containing whitelisted users
        - Hash key: userid
        - Range key: servicename
        - Other attributes: None
      - Table containing user info:
        - Hash key: uid
        - Range key: pwd
        - Other attributes: name, last date of registration, list of authorized services.

#### Usage:
    - Integrate this library by linking libauth.so to your application.
    - Instantiate the auth service by creating the auth object with the necessary json:
    

<!-- HTML generated using hilite.me --><div style="background: #ffffff; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%">{
    <span style="color: #007700">&quot;WhitelistedServicesTable&quot;</span> : <span style="background-color: #fff0f0">&quot;TABLENAME&quot;</span>,
    <span style="color: #007700">&quot;WhitelistedUsersTable&quot;</span> : <span style="background-color: #fff0f0">&quot;TABLENAME&quot;</span>,
    <span style="color: #007700">&quot;AuthTable&quot;</span> : <span style="background-color: #fff0f0">&quot;TABLENAME&quot;</span>
}
</pre></div>
